#include "renderer.hpp"
#include <exception>
#include <iostream>

int main() {
	Renderer engine;

	try {
		engine.run();
	} catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
		return -1;
	}

	return 0;
}
