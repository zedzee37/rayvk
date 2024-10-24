#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#include <stdbool.h>

#define CHECK_ERROR(error) \
	if ((error).didError)  \
		return error;

#define DEFINE_ERR() \
	RayError error;  \
	error.didError = false;

#define ERROR(msg)         \
	error.didError = true; \
	error.errorMsg = msg;  \
	return error

typedef enum RayErrorType {
	UNKNOWN,
} RayErrorType;

typedef struct RayError {
	RayErrorType type;
	const char *errorMsg;
	bool didError;
} RayError;

#endif
