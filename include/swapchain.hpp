#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include <GLFW/glfw3.h>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace renderer {
struct SwapchainSupportDetails {
	vk::SurfaceCapabilitiesKHR capabilities;
	std::vector<vk::SurfaceFormatKHR> formats;
	std::vector<vk::PresentModeKHR> presentModes;

	static SwapchainSupportDetails querySwapchainSupport(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
};

struct Swapchain {
	static vk::SurfaceFormatKHR chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats);
	static vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes);
	static vk::Extent2D getExtent(vk::SurfaceCapabilitiesKHR capabilities, GLFWwindow *window);
};

}; //namespace renderer

#endif
