#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "device.hpp"
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace renderer {
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

class Renderer {
public:
	void run();

private:
	GLFWwindow *window;
	vk::Instance instance;
	vk::DispatchLoaderDynamic loader;
	vk::DebugUtilsMessengerEXT debugMessenger;
	vk::SurfaceKHR surface;

	vk::PhysicalDevice physicalDevice;
	vk::Device logicalDevice;
	QueueFamilyIndices indices;
	vk::Queue graphicsQueue;
	vk::Queue presentQueue;

	vk::SwapchainKHR swapchain;
	vk::Format imageFormat;
	vk::Extent2D swapchainExtent;
	std::vector<vk::Image> swapchainImages;
	std::vector<vk::ImageView> swapchainImageViews;

	void init();
	void loop();
	void cleanup();

	void initWindow();
	void initInstance();
	void initValidationLayers();
	void initSurface();
	void pickPhysicalDevice();
	void initLogicalDevice();
	void initSwapchain();
	void createImageViews();

	vk::DebugUtilsMessengerCreateInfoEXT getMessengerCreateInfo() const;

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
			void *pUserData);
};
}; //namespace renderer

#endif
