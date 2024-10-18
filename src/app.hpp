#include <vector>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

const std::vector<const char *> REQUIRED_EXTENSIONS = {
	vk::KHRSwapchainExtensionName
};
const std::vector<const char *> VALIDATION_LAYERS = {
	"VK_LAYER_KHRONOS_validation"
};

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
	vk::Instance instance;
	vk::DebugUtilsMessengerEXT debug_messenger;
	vk::PhysicalDevice physical_device;
	vk::Device device;

	void init();
	void loop();
	void draw();

	void init_glfw();
	void init_instance();
	void init_validation_layers();

	vk::DebugUtilsMessengerCreateInfoEXT get_messenger_create_info();
};
