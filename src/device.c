#include "app.h"
#include "core_types.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

bool checkDeviceExtensionSupport(VkPhysicalDevice device, const DeviceRequirements *deviceRequirements) {
	uint32_t supportedExtensionCount;
	vkEnumerateDeviceExtensionProperties(device, NULL, &supportedExtensionCount, NULL);

	VkExtensionProperties *supportedExtensions = malloc(sizeof(VkExtensionProperties) * supportedExtensionCount);
	vkEnumerateDeviceExtensionProperties(device, NULL, &supportedExtensionCount, supportedExtensions);

	for (uint32_t i = 0; i < deviceRequirements->extensionCount; i++) {
		const char *requiredExtension = deviceRequirements->extensions[i];

		bool extensionSupported = false;
		for (uint32_t j = 0; j < supportedExtensionCount; j++) {
			VkExtensionProperties supportedExtension = supportedExtensions[j];

			if (strcmp(requiredExtension, supportedExtension.extensionName) == 0) {
				extensionSupported = true;
			}
		}

		if (!extensionSupported) {
			return false;
		}
	}

	free(supportedExtensions);
	return true;
}

bool isDeviceViable(VkPhysicalDevice physicalDevice, const DeviceRequirements *deviceRequirements) {
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice, deviceRequirements->surface);

	bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice, deviceRequirements);

	return queueFamilyIndicesIsComplete(indices) && extensionsSupported;
}

RayError pickPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance, const DeviceRequirements *deviceRequirements) {
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

		if (isDeviceViable(device, deviceRequirements)) {
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

RayError createLogicalDevice(VkDevice *device, Queues *queues, VkPhysicalDevice physicalDevice, const DeviceRequirements *deviceRequirements) {
	DEFINE_ERR();
	printf("guh");
	return error;
}
