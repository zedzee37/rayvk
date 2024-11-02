#include "swapchain.hpp"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <limits>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace renderer {

SwapchainSupportDetails SwapchainSupportDetails::querySwapchainSupport(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface) {
	SwapchainSupportDetails supportDetails;
	supportDetails.formats = physicalDevice.getSurfaceFormatsKHR(surface);
	supportDetails.presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
	supportDetails.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
	return supportDetails;
}

vk::SurfaceFormatKHR Swapchain::chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats) {
	for (const vk::SurfaceFormatKHR &availableFormat : availableFormats) {
		if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eVkColorspaceSrgbNonlinear) {
			return availableFormat;
		}
	}
	return availableFormats[0];
}

vk::PresentModeKHR Swapchain::choosePresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes) {
	for (const vk::PresentModeKHR &availablePresentMode : availablePresentModes) {
		if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
			return availablePresentMode;
		}
	}
	return vk::PresentModeKHR::eFifo;
}

vk::Extent2D Swapchain::getExtent(vk::SurfaceCapabilitiesKHR capabilities, GLFWwindow *window) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	} else {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		vk::Extent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}
}; //namespace renderer
