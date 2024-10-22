#include "core_types.h"

void setError(RayError *error, const char *msg) {
	error->didError = true;
	error->errorMsg = msg;
}
