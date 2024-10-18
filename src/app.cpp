#include "app.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_funcs.hpp>
#include <vulkan/vulkan_structs.hpp>

App::~App() {
	instance.destroy();

	glfwDestroyWindow(window);
}

void App::init() {
	init_glfw();
	init_instance();
	init_validation_layers();
	select_physical_device();
	init_logical_device();
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

	vkDeviceWaitIdle(device);
}

void App::draw() {
}

void App::init_glfw() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "guh", nullptr, nullptr);
}

void App::init_instance() {
	vk::ApplicationInfo app_info;

	app_info
			.setPApplicationName("guh")
			.setApplicationVersion(vk::makeApiVersion(0, 1, 0, 0))
			.setPEngineName("No Engine")
			.setEngineVersion(vk::makeApiVersion(0, 1, 0, 0))
			.setApiVersion(vk::ApiVersion12);

	vk::InstanceCreateInfo instance_info;
	instance_info.setPApplicationInfo(&app_info);

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

void App::select_physical_device() {
	std::vector<vk::PhysicalDevice> devices = instance.enumeratePhysicalDevices();

	for (const vk::PhysicalDevice &dev : devices) {
		if (is_physical_device_viable(dev)) {
			physical_device = dev;
			return;
		}
	}

	throw std::runtime_error("no viable physical device found!");
}

void App::init_logical_device() {
}

vk::DebugUtilsMessengerCreateInfoEXT App::get_messenger_create_info() {
	vk::DebugUtilsMessengerCreateInfoEXT messenger_info;

	messenger_info
			.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
			.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance);

	return messenger_info;
}

bool App::is_physical_device_viable(vk::PhysicalDevice phys_device) {
	return true;
}
