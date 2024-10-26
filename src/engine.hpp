#ifndef ENGINE_HPP
#define ENGINE_HPP

#define GLFW_INCLUDE_VULKAN
#include "device.hpp"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

const std::vector<const char *> REQUIRED_EXTENSIONS = {
	vk::KHRSwapchainExtensionName
};
const std::vector<const char *> VALIDATION_LAYERS = {
	"VK_LAYER_KHRONOS_validation"
};

const int WIDTH = 800;
const int HEIGHT = 600;

#ifdef NDEBUG
const bool ENABLE_VALIDATION_LAYERS = true;
#else
const bool ENABLE_VALIDATION_LAYERS = false;
#endif

class Engine {
public:
	vk::Instance instance;
	vk::SurfaceKHR surface;

	Device device;

	void run();

private:
	GLFWwindow *window;
	vk::DispatchLoaderDynamic loader;
	vk::DebugUtilsMessengerEXT debug_messenger;

	void init();
	void loop();
	void cleanup();

	void init_window();
	void init_instance();
	void init_validation_layers();
	void init_surface();
	void init_device();

	vk::DebugUtilsMessengerCreateInfoEXT get_messenger_create_info() const;
	static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
			void *pUserData);
};

#endif
