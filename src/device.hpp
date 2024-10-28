#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <optional>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() const;
	static QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
};

bool isDeviceSuitable(vk::PhysicalDevice physicalDevice, VkSurfaceKHR surface, std::vector<const char *> extensions);

#endif
