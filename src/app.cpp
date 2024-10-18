#include "app.hpp"
#include <stdexcept>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_funcs.hpp>
#include <vulkan/vulkan_structs.hpp>

App::~App() {
	instance.destroy();
}

void App::init() {
	init_instance();
	init_validation_layers();
}

void App::run() {
	init();
	loop();
}

void App::loop() {
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

vk::DebugUtilsMessengerCreateInfoEXT App::get_messenger_create_info() {
	vk::DebugUtilsMessengerCreateInfoEXT messenger_info;

	messenger_info
			.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
			.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance);

	return messenger_info;
}
