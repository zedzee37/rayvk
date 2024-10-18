#include "device_manager.hpp"
#include <optional>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_funcs.hpp>
#include <vulkan/vulkan_handles.hpp>

DeviceManager::~DeviceManager() {
	device.destroy();
}

void DeviceManager::select_physical_device(vk::Instance instance, std::vector<const char *> extensions, vk::SurfaceKHR surface) {
	std::vector<vk::PhysicalDevice> devices = instance.enumeratePhysicalDevices();

	for (const vk::PhysicalDevice &dev : devices) {
		std::optional<QueueFamilyIndices> indices = is_physical_device_viable(dev, extensions, surface);

		if (indices.has_value()) {
			physical_device = dev;
			queue_indices = indices.value();
			return;
		}
	}

	throw std::runtime_error("no viable physical device found!");
}

void DeviceManager::init_logical_device(std::vector<const char *> validation_layers, std::vector<const char *> required_extensions) {
}

QueueFamilyIndices DeviceManager::find_queue_indices(vk::PhysicalDevice phys_device, vk::SurfaceKHR surface) const {
	QueueFamilyIndices indices;
	std::vector<vk::QueueFamilyProperties> properties = phys_device.getQueueFamilyProperties();

	int i = 0;
	for (const vk::QueueFamilyProperties &queue_family : properties) {
		if (indices.is_complete()) {
			break;
		}

		if (queue_family.queueFlags & vk::QueueFlagBits::eGraphics) {
			indices.graphics_family = i;
		}

		vk::Bool32 present_supported = phys_device.getSurfaceSupportKHR(i, surface);
		if (present_supported) {
			indices.present_family = i;
		}

		i++;
	}

	return indices;
}

std::optional<QueueFamilyIndices> DeviceManager::is_physical_device_viable(vk::PhysicalDevice phys_device, std::vector<const char *> required_extensions, vk::SurfaceKHR surface) const {
	std::vector<vk::ExtensionProperties> supported_extensions = phys_device.enumerateDeviceExtensionProperties();

	for (const char *required_extension : required_extensions) {
		bool found_extension = false;
		for (const vk::ExtensionProperties &properties : supported_extensions) {
			if (strcmp(properties.extensionName, required_extension) == 0) {
				found_extension = true;
				break;
			}
		}

		if (!found_extension) {
			return std::nullopt;
		}
	}

	QueueFamilyIndices indices = find_queue_indices(phys_device, surface);

	if (!indices.is_complete()) {
		return std::nullopt;
	}

	return indices;
}

bool QueueFamilyIndices::is_complete() {
	return graphics_family.has_value() && present_family.has_value();
}
