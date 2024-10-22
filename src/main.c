#include "app.h"
#include <stdio.h>

void handleError(RayError error) {
	printf("\nEncountered Fatal Error: %s\n", error.errorMsg);
}

int main() {
	RayError error = appRun();

	if (error.didError) {
		handleError(error);
		return -1;
	}

	return 0;
}
