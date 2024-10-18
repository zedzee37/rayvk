#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>

#include <optional>

struct QueueFamilyIndices {
	std::optional<uint32_t> graphics_family;
	std::optional<uint32_t> present_family;

	bool is_complete();
};

class DeviceManager {
public:
	~DeviceManager();

	vk::PhysicalDevice physical_device;
	vk::Device device;
	QueueFamilyIndices queue_indices;

	void select_physical_device(vk::Instance instance, std::vector<const char *> required_extensions, vk::SurfaceKHR surface);
	void init_logical_device(std::vector<const char *> validation_layers, std::vector<const char *> required_extension);

private:
	std::optional<QueueFamilyIndices> is_physical_device_viable(vk::PhysicalDevice device, std::vector<const char *> required_extensions, vk::SurfaceKHR surface) const;
	QueueFamilyIndices find_queue_indices(vk::PhysicalDevice phys_device, vk::SurfaceKHR surface) const;
};
