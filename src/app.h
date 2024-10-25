#ifndef APP_H
#define APP_H

#define GLFW_INCLUDE_VULKAN
#include "core_types.h"
#include "device.h"
#include <GLFW/glfw3.h>

#define WIDTH 800
#define HEIGHT 600

extern const char *REQUIRED_EXTENSIONS[];
extern const char *VALIDATION_LAYERS[];

typedef struct ConstantStringArray {
	const uint32_t len;
	const char *data[];
} ConstantStringArray;

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
extern RayError createInstance(VkInstance *instance);
extern RayError createSurface(VkInstance instance, GLFWwindow *window, VkSurfaceKHR *surface);
extern const char **getRequiredExtensions(uint32_t *extensionCount);

#endif
