#include "device.h"
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

bool checkDeviceExtensionSupport(VkPhysicalDevice device, const char *deviceExtensions[], uint32_t deviceExtensionCount) {
	uint32_t supportedExtensionCount;
	vkEnumerateDeviceExtensionProperties(device, NULL, &supportedExtensionCount, NULL);

	VkExtensionProperties *supportedExtensions = malloc(sizeof(VkExtensionProperties) * supportedExtensionCount);
	vkEnumerateDeviceExtensionProperties(device, NULL, &supportedExtensionCount, supportedExtensions);

	for (uint32_t i = 0; i < deviceExtensionCount; i++) {
		const char *requiredExtension = deviceExtensions[i];

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

bool isDeviceViable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const char *deviceExtensions[], uint32_t deviceExtensionCount) {
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);

	bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice, deviceExtensions, deviceExtensionCount);

	return queueFamilyIndicesIsComplete(indices) && extensionsSupported;
}

RayError pickPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance, VkSurfaceKHR surface, const char *deviceExtensions[], uint32_t deviceExtensionCount) {
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

		if (isDeviceViable(device, surface, deviceExtensions, deviceExtensionCount)) {
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

RayError createLogicalDevice(VkDevice *device, Queues *queues, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const char **deviceExtensions, uint32_t deviceExtensionCount, const char **validationLayers, uint32_t validationLayerCount, bool validationEnabled) {
	DEFINE_ERR();

	QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);

	uint32_t usedQueues[] = { indices.graphicsFamily.value, indices.presentFamily.value };
	uint32_t queueCount = sizeof(usedQueues) / sizeof(uint32_t);

	uint32_t **queueSet = malloc(queueCount * sizeof(uint32_t *));
	uint32_t setLen = 0;

	for (uint32_t i = 0; i < queueCount; i++) {
		uint32_t hash = usedQueues[i] % queueCount;

		if (queueSet[hash] == NULL) {
			queueSet[hash] = &usedQueues[i];
			setLen++;
		}
	}

	VkDeviceQueueCreateInfo *queueCreateInfos = malloc(sizeof(VkDeviceQueueCreateInfo) * setLen);

	float queuePriority = 1.0f;
	for (uint32_t i = 0; i < setLen; i++) {
		uint32_t *queue = queueSet[i];
		VkDeviceQueueCreateInfo queueInfo;
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = *queue;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos[i] = queueInfo;
	}

	free(queueSet);

	return error;
}
