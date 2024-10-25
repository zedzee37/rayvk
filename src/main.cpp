#include "engine.hpp"
#include <exception>
#include <iostream>

int main() {
	Engine engine;

	try {
		engine.run();
	} catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
		return -1;
	}

	return 0;
}
