#define GLFW_INCLUDE_VULKAN
#include "core_types.h"
#include <GLFW/glfw3.h>

#define WIDTH 800
#define HEIGHT 600

typedef struct App {
	GLFWwindow *window;
	VkInstance instance;
} App;

extern RayError appRun();
extern RayError appInit(App *app);
extern RayError appLoop(App *app);
extern void appCleanup(App *app);

extern GLFWwindow *appInitGlfw();
extern RayError appCreateInstance(VkInstance *instance);
