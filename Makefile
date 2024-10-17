VULKAN_INCLUDE_PATH = /usr/include/vulkan
STB_INCLUDE_PATH = /usr/include/stb
CFLAGS = -std=c++17 -O2 -I$(VULKAN_INCLUDE_PATH) -I$(STB_INCLUDE_PATH)
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

VulkanTest: src/main.cpp
	g++ $(CFLAGS) -o VulkanTest src/*.cpp $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest

shader:
	glslc shaders/tri.vert -o shaders/vert.spv
	glslc shaders/tri.frag -o shaders/frag.spv
