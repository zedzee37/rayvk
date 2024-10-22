#include <stdbool.h>

#define CHECK_ERROR(error) \
	if ((error).didError)  \
		return error;

#define DEFINE_ERR() \
	RayError error;  \
	error.didError = false;

typedef enum RayErrorType {
	UNKNOWN,
} RayErrorType;

typedef struct RayError {
	RayErrorType type;
	char *errorMsg;
	bool didError;
} RayError;
