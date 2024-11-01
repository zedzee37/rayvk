#include "device.hpp"
#include <cstring>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace renderer {
bool QueueFamilyIndices::isComplete() const {
	return graphicsFamily.has_value() && presentFamily.has_value();
}

QueueFamilyIndices QueueFamilyIndices::findQueueFamilies(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface) {
	QueueFamilyIndices indices;
	std::vector<vk::QueueFamilyProperties> properties = physicalDevice.getQueueFamilyProperties();

	int i = 0;
	for (const vk::QueueFamilyProperties &queue_family : properties) {
		if (indices.isComplete()) {
			break;
		}

		if (queue_family.queueFlags & vk::QueueFlagBits::eGraphics) {
			indices.graphicsFamily = i;
		}

		vk::Bool32 present_supported = physicalDevice.getSurfaceSupportKHR(i, surface);
		if (present_supported) {
			indices.presentFamily = i;
		}

		i++;
	}

	return indices;
}

bool isDeviceSuitable(vk::PhysicalDevice physicalDevice, VkSurfaceKHR surface, std::vector<const char *> extensions) {
	std::vector<vk::ExtensionProperties> supportedExtensions = physicalDevice.enumerateDeviceExtensionProperties();

	for (const char *requiredExtension : extensions) {
		bool foundExtension = false;

		for (const vk::ExtensionProperties &supportedExtension : supportedExtensions) {
			if (strcmp(supportedExtension.extensionName, requiredExtension) == 0) {
				foundExtension = true;
				break;
			}
		}

		if (!foundExtension) {
			return false;
		}
	}

	QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(physicalDevice, surface);

	return indices.isComplete();
}
}; //namespace renderer
