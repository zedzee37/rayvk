#include "app.h"

RayError appRun() {
	DEFINE_ERR();
	App app;

	error = appInit(&app);
	CHECK_ERROR(error);

	error = appLoop(&app);
	CHECK_ERROR(error);

	error = appCleanup(&app);
	CHECK_ERROR(error);

	return error;
}

RayError appInit(App *app) {
	DEFINE_ERR();
	return error;
}

RayError appLoop(App *app) {
	DEFINE_ERR();
	return error;
}

RayError appCleanup(App *app) {
	DEFINE_ERR();
	return error;
}
