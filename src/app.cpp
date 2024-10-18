#include "app.hpp"
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_funcs.hpp>
#include <vulkan/vulkan_structs.hpp>

App::~App() {
}

void App::run() {
	init();
	loop();
}

void App::init() {
	init_instance();
}

void App::init_instance() {
	vk::ApplicationInfo app_info;
	app_info.pApplicationName = "guh";
	app_info.applicationVersion = vk::makeApiVersion(0, 1, 0, 0);
	app_info.pEngineName = "No Engine";
	app_info.engineVersion = vk::makeApiVersion(0, 1, 0, 0);
	app_info.apiVersion = vk::ApiVersion12;

	vk::InstanceCreateInfo instance_info;
	instance_info.pApplicationInfo = &app_info;

	instance = vk::createInstance(instance_info);
}
