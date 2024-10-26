#ifndef APP_H
#define APP_H

#define GLFW_INCLUDE_VULKAN
#include "core_types.h"
#include "optional.h"
#include <GLFW/glfw3.h>

#define WIDTH 800
#define HEIGHT 600

extern const char *REQUIRED_EXTENSIONS[];
extern const char *VALIDATION_LAYERS[];

typedef struct ConstantStringArray {
	const uint32_t len;
	const char *data[];
} ConstantStringArray;

typedef struct QueueFamilyIndices {
	uint32_t_optional graphicsFamily;
	uint32_t_optional presentFamily;
} QueueFamilyIndices;

extern bool queueFamilyIndicesIsComplete(QueueFamilyIndices indices);

typedef struct DeviceRequirements {
	const char **extensions;
	uint32_t extensionCount;
	const char **validationLayers;
	uint32_t validationLayerCount;
	VkSurfaceKHR surface;
} DeviceRequirements;

typedef struct Queues {
	VkQueue graphics;
	VkQueue present;
} Queues;

typedef struct App {
	GLFWwindow *window;
	VkInstance instance;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	Queues queues;
} App;

extern RayError appRun();
extern RayError appInit(App *app);
extern RayError appLoop(App *app);
extern void appCleanup(App *app);
extern GLFWwindow *appInitGlfw();
extern RayError appInitDevice(App *app);

extern RayError createInstance(VkInstance *instance);
extern RayError createSurface(VkInstance instance, GLFWwindow *window, VkSurfaceKHR *surface);
extern const char **getRequiredExtensions(uint32_t *extensionCount);

extern QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
extern bool checkDeviceExtensionSupport(VkPhysicalDevice device, const DeviceRequirements *deviceRequirements);
extern bool isDeviceViable(VkPhysicalDevice physicalDevice, const DeviceRequirements *deviceRequirements);
extern RayError pickPhysicalDevice(VkPhysicalDevice *physicalDevice, VkInstance instance, const DeviceRequirements *deviceRequirements);
extern RayError createLogicalDevice(VkDevice *device, Queues *queues, VkPhysicalDevice physicalDevice, const DeviceRequirements *deviceRequirements);

extern RayError appDeviceInit();

#endif
