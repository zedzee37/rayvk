#include <cwchar>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

#include "device_manager.hpp"

#include <GLFW/glfw3.h>

const std::vector<const char *> REQUIRED_EXTENSIONS = {
	vk::KHRSwapchainExtensionName
};
const std::vector<const char *> VALIDATION_LAYERS = {
	"VK_LAYER_KHRONOS_validation"
};
const std::vector<const char *> GLFW_EXTENSIONS = {
	vk::KHRSurfaceExtensionName,
};

const int WIDTH = 800;
const int HEIGHT = 600;

#ifdef NDEBUG
const bool enable_validation_layers = true;
#else
const bool enable_validation_layers = false;
#endif

class App {
public:
	~App();

	void run();

private:
	GLFWwindow *window;
	vk::Instance instance;
	vk::DebugUtilsMessengerEXT debug_messenger;
	DeviceManager device_manager;
	vk::SurfaceKHR surface;

	void init();
	void loop();
	void draw();

	void init_glfw();
	void init_instance();
	void init_validation_layers();
	void init_surface();

	vk::DebugUtilsMessengerCreateInfoEXT get_messenger_create_info() const;
};
