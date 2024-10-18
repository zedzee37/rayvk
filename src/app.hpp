#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>

class App {
public:
	~App();

	void run();

private:
	vk::Instance instance;
	vk::PhysicalDevice physical_device;
	vk::Device device;

	void init();
	void loop();
	void draw();

	void init_glfw();
	void init_instance();
};
