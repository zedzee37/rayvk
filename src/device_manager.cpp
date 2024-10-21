#include "device_manager.hpp"
#include <cstdint>
#include <optional>
#include <set>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_funcs.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

void DeviceManager::cleanup() {
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

Queues DeviceManager::init_logical_device(vk::Instance instance, vk::PhysicalDeviceFeatures required_features, std::vector<const char *> validation_layers, std::vector<const char *> required_extensions, bool use_validation_layers) {
	std::vector<vk::DeviceQueueCreateInfo> queue_infos;
	std::set<uint32_t> unique_queue_families = {
		queue_indices.graphics_family.value(), queue_indices.present_family.value()
	};

	float queue_priority = 1.0f;
	for (uint32_t queue_family : unique_queue_families) {
		vk::DeviceQueueCreateInfo queue_info;
		queue_info
				.setQueueFamilyIndex(queue_family)
				.setQueueCount(1)
				.setPQueuePriorities(&queue_priority);
		queue_infos.push_back(queue_info);
	}

	vk::PhysicalDeviceDescriptorIndexingFeatures descriptor_indexing_features;

	descriptor_indexing_features
			.setShaderSampledImageArrayNonUniformIndexing(vk::True)
			.setShaderUniformBufferArrayNonUniformIndexing(vk::True)
			.setRuntimeDescriptorArray(vk::True)
			.setDescriptorBindingVariableDescriptorCount(vk::True)
			.setDescriptorBindingPartiallyBound(vk::True);

	vk::DeviceCreateInfo device_info;
	device_info
			.setQueueCreateInfoCount(queue_infos.size())
			.setPQueueCreateInfos(queue_infos.data())
			.setPEnabledFeatures(&required_features)
			.setEnabledExtensionCount(required_extensions.size())
			.setPEnabledExtensionNames(required_extensions)
			.setPNext(&descriptor_indexing_features);

	if (use_validation_layers) {
		device_info
				.setEnabledLayerCount(validation_layers.size())
				.setPEnabledLayerNames(validation_layers);
	} else {
		device_info.setEnabledLayerCount(0);
	}

	device = physical_device.createDevice(device_info);

	Queues queues;
	queues.graphics = device.getQueue(queue_indices.graphics_family.value(), 0);
	queues.present = device.getQueue(queue_indices.present_family.value(), 0);
	return queues;
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
