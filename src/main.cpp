#include "app.hpp"
#include <exception>
#include <iostream>

int main() {
	App app;

	try {
		app.run();
	} catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
		return -1;
	}

	return 0;
}
