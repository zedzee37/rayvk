#include "engine.hpp"
#include "device.hpp"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

void Engine::run() {
	init();
	loop();
	cleanup();
}

void Engine::init() {
	init_window();
	init_instance();
	init_validation_layers();
	init_surface();
	init_device();
}

void Engine::loop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void Engine::cleanup() {
	instance.destroySurfaceKHR(surface);
	instance.destroyDebugUtilsMessengerEXT(debug_messenger, nullptr, loader);

	glfwDestroyWindow(window);
	glfwTerminate();

	instance.destroy();
}

void Engine::init_window() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "guh", nullptr, nullptr);

	glfwSetErrorCallback([](int error, const char *description) {
		std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
	});
}

void Engine::init_instance() {
	vk::ApplicationInfo app_info;

	app_info
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

	vk::InstanceCreateInfo instance_info;
	instance_info
			.setPApplicationInfo(&app_info)
			.setEnabledExtensionCount(extensions.size())
			.setPEnabledExtensionNames(extensions);

	if (ENABLE_VALIDATION_LAYERS) {
		instance_info
				.setEnabledLayerCount(VALIDATION_LAYERS.size())
				.setPEnabledLayerNames(VALIDATION_LAYERS);

		vk::DebugUtilsMessengerCreateInfoEXT debug_info = get_messenger_create_info();
		instance_info.pNext = &debug_info;
	} else {
		instance_info.setEnabledLayerCount(0);
		instance_info.pNext = nullptr;
	}

	try {
		instance = vk::createInstance(instance_info);
	} catch (const vk::SystemError &err) {
		std::cerr << "Failed to create Vulkan instance: " << err.what() << std::endl;
		return;
	}

	vk::DynamicLoader dyna_loader;
	auto get_func = dyna_loader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
	loader.init(instance, get_func);
}

void Engine::init_surface() {
	VkSurfaceKHR temp_surface;

	if (glfwCreateWindowSurface(static_cast<VkInstance>(instance), window, nullptr, &temp_surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}

	surface = temp_surface;
}

void Engine::init_device() {
	device = Device(this);

	device.pick_physical_device();
	device.init_logical_device();
	Queues queues = device.get_device_queues();
}

vk::DebugUtilsMessengerCreateInfoEXT Engine::get_messenger_create_info() const {
	vk::DebugUtilsMessengerCreateInfoEXT messenger_info;

	messenger_info
			.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
			.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
			.setPfnUserCallback(Engine::debug_callback);

	return messenger_info;
}

void Engine::init_validation_layers() {
	if (!ENABLE_VALIDATION_LAYERS) {
		return;
	}

	vk::DebugUtilsMessengerCreateInfoEXT messenger_info = get_messenger_create_info();

	if (instance.createDebugUtilsMessengerEXT(&messenger_info, nullptr, &debug_messenger, loader) != vk::Result::eSuccess) {
		throw std::runtime_error("failed to create debug messenger.");
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL Engine::debug_callback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
		void *pUserData) {
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}
