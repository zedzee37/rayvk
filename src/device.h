#include "core_types.h"
#include "optional.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

typedef struct QueueFamilyIndices {
	uint32_t_optional graphicsFamily;
	uint32_t_optional presentFamily;
} QueueFamilyIndices;

extern bool queueFamilyIndicesIsComplete(QueueFamilyIndices indices);

extern QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
extern bool isDeviceViable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
extern RayError pickPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance, VkSurfaceKHR surface);

extern RayError appDeviceInit();
