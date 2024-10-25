#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <optional>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>

struct QueueFamilyIndices {
	std::optional<uint32_t> graphics_family;
	std::optional<uint32_t> present_family;

	bool is_complete() const;
	static QueueFamilyIndices find_queue_families(vk::PhysicalDevice physical_device, vk::SurfaceKHR surface);
};

struct Queues {
	vk::Queue graphics;
	vk::Queue present;
};

class Device {
public:
	vk::PhysicalDevice physical_device;
	vk::Device logical_device;
	QueueFamilyIndices indices;

	void cleanup();
	void pick_physical_device();
	void init_logical_device();
	Queues get_device_queues();

private:
	vk::Instance instance;
	vk::SurfaceKHR surface;
	std::vector<const char *> required_extensions;
	std::vector<const char *> validation_layers;
	bool enable_validation_layers;
};

bool is_device_suitable(vk::PhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<const char *> extensions);

#endif
