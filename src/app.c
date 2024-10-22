#include "app.h"
#include <GLFW/glfw3.h>

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

	return error;
}
