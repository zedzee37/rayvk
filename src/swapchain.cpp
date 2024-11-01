#include "swapchain.hpp"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <limits>
#include <vulkan/vulkan_structs.hpp>

namespace renderer {

SwapchainSupportDetails SwapchainSupportDetails::querySwapchainSupport(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface) {
	SwapchainSupportDetails supportDetails;
	supportDetails.formats = physicalDevice.getSurfaceFormatsKHR(surface);
	supportDetails.presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
	supportDetails.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
	return supportDetails;
}

VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
	for (const VkSurfaceFormatKHR &availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}
	return availableFormats[0];
}

VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
	for (const VkPresentModeKHR &availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	} else {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

}; //namespace renderer
