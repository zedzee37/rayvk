#include "app.hpp"
#include <GLFW/glfw3.h>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_funcs.hpp>
#include <vulkan/vulkan_structs.hpp>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

App::~App() {
	instance.destroySurfaceKHR(surface);
	instance.destroyDebugUtilsMessengerEXT(debug_messenger, nullptr);

	glfwDestroyWindow(window);
}

void App::init() {
	init_glfw();
	init_instance();
	init_validation_layers();
	init_surface();
	device_manager.select_physical_device(instance, REQUIRED_EXTENSIONS, surface);
	device_manager.init_logical_device(VALIDATION_LAYERS, REQUIRED_EXTENSIONS);
}

void App::run() {
	init();
	loop();
}

void App::loop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		draw();
	}
}

void App::draw() {
}

void App::init_glfw() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "guh", nullptr, nullptr);

	glfwSetErrorCallback([](int error, const char *description) {
		std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
	});
}

void App::init_instance() {
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

	if (enable_validation_layers) {
		extensions.push_back(vk::EXTDebugUtilsExtensionName);
	}

	vk::InstanceCreateInfo instance_info;
	instance_info
			.setPApplicationInfo(&app_info)
			.setEnabledExtensionCount(extensions.size())
			.setPEnabledExtensionNames(extensions);

	if (enable_validation_layers) {
		instance_info
				.setEnabledLayerCount(VALIDATION_LAYERS.size())
				.setPEnabledLayerNames(VALIDATION_LAYERS);
	} else {
		instance_info.setEnabledLayerCount(0);
	}

	try {
		instance = vk::createInstance(instance_info);
	} catch (const vk::SystemError &err) {
		std::cerr << "Failed to create Vulkan instance: " << err.what() << std::endl;
		return;
	}

	VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
	VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);
}

void App::init_validation_layers() {
	if (!enable_validation_layers) {
		return;
	}

	vk::DebugUtilsMessengerCreateInfoEXT messenger_info;

	messenger_info
			.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
			.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
			.setPfnUserCallback(App::debug_callback);

	if (instance.createDebugUtilsMessengerEXT(&messenger_info, nullptr, &debug_messenger) != vk::Result::eSuccess) {
		throw std::runtime_error("failed to create debug messenger.");
	}
}

void App::init_surface() {
	VkSurfaceKHR temp_surface;
	if (glfwCreateWindowSurface(static_cast<VkInstance>(instance), window, nullptr, &temp_surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}

	surface = temp_surface;
}

vk::DebugUtilsMessengerCreateInfoEXT App::get_messenger_create_info() const {
	vk::DebugUtilsMessengerCreateInfoEXT messenger_info;

	messenger_info
			.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
			.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
			.setPfnUserCallback(App::debug_callback);

	return messenger_info;
}

VKAPI_ATTR VkBool32 VKAPI_CALL App::debug_callback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
		void *pUserData) {
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}
