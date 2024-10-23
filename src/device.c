#include "device.h"
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

bool queueFamilyIndicesIsComplete(QueueFamilyIndices indices) {
	return indices.graphicsFamily.hasValue && indices.presentFamily.hasValue;
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

	VkQueueFamilyProperties *queueFamilies = malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

	for (uint32_t i = 0; i < queueFamilyCount; i++) {
		if (queueFamilyIndicesIsComplete(indices)) {
			break;
		}

		VkQueueFamilyProperties queueFamily = queueFamilies[i];

		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			ASSIGN_OPTIONAL(indices.graphicsFamily, i);
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (presentSupport) {
			ASSIGN_OPTIONAL(indices.presentFamily, i);
		}
	}

	free(queueFamilies);
	return indices;
}

bool isDeviceViable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
	return true;
}

RayError pickPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance, VkSurfaceKHR surface) {
	DEFINE_ERR();

	uint32_t physicalDeviceCount;
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, NULL);

	if (physicalDeviceCount == 0) {
		ERROR("there are no physical devices which support vulkan!");
	}

	VkPhysicalDevice *physicalDevices = malloc(sizeof(VkPhysicalDevice) * physicalDeviceCount);
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices);

	for (uint32_t i = 0; i < physicalDeviceCount; i++) {
		VkPhysicalDevice device = physicalDevices[i];

		if (isDeviceViable(device, surface)) {
			*physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == NULL) {
		ERROR("failed to find a suitable physical device!");
	}

	free(physicalDevices);
	return error;
}
