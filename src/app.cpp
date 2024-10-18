#include "app.hpp"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_funcs.hpp>
#include <vulkan/vulkan_structs.hpp>

App::~App() {
	instance.destroySurfaceKHR(surface);
	instance.destroy();

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

	vkDeviceWaitIdle(device_manager.device);
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

	const std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

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

	instance = vk::createInstance(instance_info);
}

void App::init_validation_layers() {
	if (!enable_validation_layers) {
		return;
	}

	vk::DebugUtilsMessengerCreateInfoEXT messenger_info = get_messenger_create_info();

	if (instance.createDebugUtilsMessengerEXT(&messenger_info, nullptr, &debug_messenger) != vk::Result::eSuccess) {
		throw std::runtime_error("failed to initialize debug messenger!");
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
			.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance);

	return messenger_info;
}
