#include "app.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

const char *REQUIRED_EXTENSIONS[] = {

};
const char *VALIDATION_LAYERS[] = {
	"VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool ENABLE_VALIDATION = true;
#else
const bool ENABLE_VALIDATION = false;
#endif

RayError appRun() {
	DEFINE_ERR();
	App app;

	error = appInit(&app);
	CHECK_ERROR(error);

	error = appLoop(&app);
	CHECK_ERROR(error);

	appCleanup(&app);
	return error;
}

RayError appInit(App *app) {
	DEFINE_ERR();

	app->window = appInitGlfw();

	error = appCreateInstance(&app->instance);
	CHECK_ERROR(error);

	return error;
}

RayError appLoop(App *app) {
	DEFINE_ERR();

	while (true) {
		glfwPollEvents();
	}

	return error;
}

void appCleanup(App *app) {
	vkDestroyInstance(app->instance, NULL);

	glfwDestroyWindow(app->window);
}

GLFWwindow *appInitGlfw() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow *window;
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", NULL, NULL);
	return window;
}

RayError appCreateInstance(VkInstance *instance) {
	DEFINE_ERR();

	VkApplicationInfo appInfo;
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "epic raytracer 9000";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_1;

	uint32_t extensionCount;
	const char **extensions = getRequiredExtensions(&extensionCount);

	VkInstanceCreateInfo instanceInfo = {};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledExtensionCount = extensionCount;
	instanceInfo.ppEnabledExtensionNames = extensions;

	if (ENABLE_VALIDATION) {
		int layerCount = sizeof(VALIDATION_LAYERS) / sizeof(VALIDATION_LAYERS[0]);
		instanceInfo.enabledLayerCount = layerCount;
		instanceInfo.ppEnabledLayerNames = VALIDATION_LAYERS;
	} else {
		instanceInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&instanceInfo, NULL, instance) != VK_SUCCESS) {
		ERROR("failed to create instance.");
	}

	return error;
}

const char **getRequiredExtensions(uint32_t *extensionCount) {
	const char **glfwExtensions;
	uint32_t glfwExtensionCount = 0;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	*extensionCount = glfwExtensionCount + 1;
	const char **requiredExtensions = malloc(sizeof(const char *) * *extensionCount);

	for (uint32_t i = 0; i < *extensionCount - 1; i++) {
		requiredExtensions[i] = glfwExtensions[i];
	}

	if (ENABLE_VALIDATION) {
		requiredExtensions[*extensionCount - 1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
	}

	return requiredExtensions;
}
