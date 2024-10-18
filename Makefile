VULKAN_INCLUDE_PATH = /usr/include/vulkan
stb_include_path = /usr/include/stb
cflags = -std=c++17 -o2 -i$(vulkan_include_path) -i$(stb_include_path)
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
