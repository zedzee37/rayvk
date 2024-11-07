#include "renderer.hpp"
#include "device.hpp"
#include "image.hpp"
#include "swapchain.hpp"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <set>
#include <stdexcept>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace renderer {
void Renderer::run() {
	init();
	loop();
	cleanup();
}

void Renderer::init() {
	initWindow();
	initInstance();
	initValidationLayers();
	initSurface();
	pickPhysicalDevice();
	initLogicalDevice();
	initSwapchain();
	// createImageViews();
}

void Renderer::loop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void Renderer::cleanup() {
	logicalDevice.destroy();
	instance.destroySurfaceKHR(surface);
	instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, loader);

	glfwDestroyWindow(window);
	glfwTerminate();

	instance.destroy();
}

void Renderer::initWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "guh", nullptr, nullptr);

	glfwSetErrorCallback([](int error, const char *description) {
		std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
	});
}

void Renderer::initInstance() {
	vk::ApplicationInfo appInfo;

	appInfo
			.setPApplicationName("guh")
			.setApplicationVersion(vk::makeApiVersion(0, 1, 0, 0))
			.setPEngineName("No Engine")
			.setEngineVersion(vk::makeApiVersion(0, 1, 0, 0))
			.setApiVersion(vk::ApiVersion12);

	uint32_t glfwExtensionCount = 0;
	const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (ENABLE_VALIDATION_LAYERS) {
		extensions.push_back(vk::EXTDebugUtilsExtensionName);
	}

	vk::InstanceCreateInfo instanceInfo;
	instanceInfo
			.setPApplicationInfo(&appInfo)
			.setEnabledExtensionCount(extensions.size())
			.setPEnabledExtensionNames(extensions);

	if (ENABLE_VALIDATION_LAYERS) {
		instanceInfo
				.setEnabledLayerCount(VALIDATION_LAYERS.size())
				.setPEnabledLayerNames(VALIDATION_LAYERS);

		vk::DebugUtilsMessengerCreateInfoEXT debug_info = getMessengerCreateInfo();
		instanceInfo.pNext = &debug_info;
	} else {
		instanceInfo.setEnabledLayerCount(0);
		instanceInfo.pNext = nullptr;
	}

	try {
		instance = vk::createInstance(instanceInfo);
	} catch (const vk::SystemError &err) {
		std::cerr << "Failed to create Vulkan instance: " << err.what() << std::endl;
		return;
	}

	vk::DynamicLoader dynaLoader;
	auto getFunc = dynaLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
	loader.init(instance, getFunc);
}

void Renderer::initSurface() {
	VkSurfaceKHR tempSurface;

	if (glfwCreateWindowSurface(static_cast<VkInstance>(instance), window, nullptr, &tempSurface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}

	surface = tempSurface;
}

void Renderer::pickPhysicalDevice() {
	std::vector<vk::PhysicalDevice> possibleDevices = instance.enumeratePhysicalDevices();

	for (const vk::PhysicalDevice &possibleDevice : possibleDevices) {
		if (isDeviceSuitable(possibleDevice, surface, REQUIRED_EXTENSIONS)) {
			indices = QueueFamilyIndices::findQueueFamilies(possibleDevice, surface);
			physicalDevice = possibleDevice;
			return;
		}
	}

	throw std::runtime_error("could not find suitable physical device!");
}

void Renderer::initLogicalDevice() {
	std::vector<vk::DeviceQueueCreateInfo> queueInfos;
	std::set<uint32_t> uniqueQueueFamilies = {
		indices.graphicsFamily.value(), indices.presentFamily.value()
	};

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		vk::DeviceQueueCreateInfo queueInfo;
		queueInfo
				.setQueueFamilyIndex(queueFamily)
				.setQueueCount(1)
				.setPQueuePriorities(&queuePriority);
		queueInfos.push_back(queueInfo);
	}

	vk::PhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures;

	descriptorIndexingFeatures
			.setShaderSampledImageArrayNonUniformIndexing(vk::True)
			.setShaderUniformBufferArrayNonUniformIndexing(vk::True)
			.setRuntimeDescriptorArray(vk::True)
			.setDescriptorBindingVariableDescriptorCount(vk::True)
			.setDescriptorBindingPartiallyBound(vk::True);

	vk::PhysicalDeviceFeatures requiredFeatures;
	requiredFeatures.setSamplerAnisotropy(vk::True);

	vk::DeviceCreateInfo deviceInfo;
	deviceInfo
			.setQueueCreateInfoCount(queueInfos.size())
			.setPQueueCreateInfos(queueInfos.data())
			.setPEnabledFeatures(&requiredFeatures)
			.setEnabledExtensionCount(REQUIRED_EXTENSIONS.size())
			.setPEnabledExtensionNames(REQUIRED_EXTENSIONS)
			.setPNext(&descriptorIndexingFeatures);

	if (ENABLE_VALIDATION_LAYERS) {
		deviceInfo
				.setEnabledLayerCount(VALIDATION_LAYERS.size())
				.setPEnabledLayerNames(VALIDATION_LAYERS);
	} else {
		deviceInfo.setEnabledLayerCount(0);
	}

	logicalDevice = physicalDevice.createDevice(deviceInfo);

	graphicsQueue = logicalDevice.getQueue(indices.graphicsFamily.value(), 0);
	presentQueue = logicalDevice.getQueue(indices.presentFamily.value(), 0);
}

void Renderer::initSwapchain() {
	SwapchainSupportDetails supportDetails = SwapchainSupportDetails::querySwapchainSupport(physicalDevice, surface);

	vk::SurfaceFormatKHR surfaceFormat = Swapchain::chooseSurfaceFormat(supportDetails.formats);
	vk::PresentModeKHR presentMode = Swapchain::choosePresentMode(supportDetails.presentModes);
	vk::Extent2D extent = Swapchain::getExtent(supportDetails.capabilities, window);

	imageFormat = surfaceFormat.format;
	swapchainExtent = extent;

	uint32_t imageCount = supportDetails.capabilities.minImageCount + 1;
	if (supportDetails.capabilities.maxImageCount > 0 && imageCount > supportDetails.capabilities.maxImageCount) {
		imageCount = supportDetails.capabilities.maxImageCount;
	}

	vk::SwapchainCreateInfoKHR swapchainInfo{};
	swapchainInfo
			.setSurface(surface)
			.setMinImageCount(imageCount)
			.setImageFormat(imageFormat)
			.setImageColorSpace(surfaceFormat.colorSpace)
			.setImageExtent(swapchainExtent)
			.setImageArrayLayers(1)
			.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);

	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		swapchainInfo
				.setImageSharingMode(vk::SharingMode::eConcurrent)
				.setQueueFamilyIndexCount(2)
				.setQueueFamilyIndices(queueFamilyIndices);
	} else {
		swapchainInfo
				.setImageSharingMode(vk::SharingMode::eExclusive);
	}

	swapchainInfo
			.setPreTransform(supportDetails.capabilities.currentTransform)
			.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
			.setPresentMode(presentMode)
			.setClipped(vk::True);

	swapchain = logicalDevice.createSwapchainKHR(swapchainInfo);
	swapchainImages = logicalDevice.getSwapchainImagesKHR(swapchain);
}

void Renderer::createImageViews() {
	for (size_t i = 0; i < swapchainImages.size(); i++) {
		swapchainImageViews[i] = Image::createImageView(logicalDevice, swapchainImages[i], imageFormat, vk::ImageAspectFlagBits::eColor);
	}
}

vk::DebugUtilsMessengerCreateInfoEXT Renderer::getMessengerCreateInfo() const {
	vk::DebugUtilsMessengerCreateInfoEXT messengerInfo;

	messengerInfo
			.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
			.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
			.setPfnUserCallback(Renderer::debugCallback);

	return messengerInfo;
}

void Renderer::initValidationLayers() {
	if (!ENABLE_VALIDATION_LAYERS) {
		return;
	}

	vk::DebugUtilsMessengerCreateInfoEXT messengerInfo = getMessengerCreateInfo();

	if (instance.createDebugUtilsMessengerEXT(&messengerInfo, nullptr, &debugMessenger, loader) != vk::Result::eSuccess) {
		throw std::runtime_error("failed to create debug messenger.");
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL Renderer::debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
		void *pUserData) {
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}
}; //namespace renderer
