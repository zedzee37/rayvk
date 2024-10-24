#ifndef DEVICE_H
#define DEVICE_H

#include "core_types.h"
#include "optional.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

typedef struct QueueFamilyIndices {
	uint32_t_optional graphicsFamily;
	uint32_t_optional presentFamily;
} QueueFamilyIndices;

typedef struct Queues {
	VkQueue graphics;
	VkQueue present;
} Queues;

extern bool queueFamilyIndicesIsComplete(QueueFamilyIndices indices);

extern QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
extern bool checkDeviceExtensionSupport(VkPhysicalDevice device, const char *deviceExtensions[], uint32_t deviceExtensionCount);
extern bool isDeviceViable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const char *deviceExtensions[], uint32_t deviceExtensionCount);
extern RayError pickPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance, VkSurfaceKHR surface, const char *deviceExtensions[], uint32_t deviceExtensionCount);
extern RayError createLogicalDevice(VkDevice *device, Queues *queues, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const char *deviceExtensions[], uint32_t deviceExtensionCount, const char *validationLayers[], uint32_t validationLayerCount, bool validationEnabled);

extern RayError appDeviceInit();

#endif
