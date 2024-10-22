#define GLFW_INCLUDE_VULKAN
#include "core_types.h"
#include <GLFW/glfw3.h>

typedef struct App {
	VkInstance instance;
} App;

extern RayError appRun();
extern RayError appInit(App *app);
extern RayError appLoop(App *app);
extern RayError appCleanup(App *app);
