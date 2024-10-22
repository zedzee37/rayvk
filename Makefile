VULKAN_INCLUDE_PATH = /usr/include/vulkan
STB_INCLUDE_PATH = /usr/include/stb
CFLAGS = -std=c17 -g -O0 -I$(VULKAN_INCLUDE_PATH) -I$(STB_INCLUDE_PATH)
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -DNDEBUG=1

VulkanTest: src/*.c
	gcc $(CFLAGS) -o VulkanTest src/*.c $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest

shader:
	glslc shaders/tri.vert -o shaders/vert.spv
	glslc shaders/tri.frag -o shaders/frag.spv
