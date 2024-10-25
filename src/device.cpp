#include "device.hpp"
#include <cstring>
#include <set>
#include <stdexcept>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

bool QueueFamilyIndices::is_complete() const {
	return graphics_family.has_value() && present_family.has_value();
}

QueueFamilyIndices QueueFamilyIndices::find_queue_families(vk::PhysicalDevice physical_device, vk::SurfaceKHR surface) {
	QueueFamilyIndices indices;
	std::vector<vk::QueueFamilyProperties> properties = physical_device.getQueueFamilyProperties();

	int i = 0;
	for (const vk::QueueFamilyProperties &queue_family : properties) {
		if (indices.is_complete()) {
			break;
		}

		if (queue_family.queueFlags & vk::QueueFlagBits::eGraphics) {
			indices.graphics_family = i;
		}

		vk::Bool32 present_supported = physical_device.getSurfaceSupportKHR(i, surface);
		if (present_supported) {
			indices.present_family = i;
		}

		i++;
	}

	return indices;
}

bool is_device_suitable(vk::PhysicalDevice physical_device, VkSurfaceKHR surface, std::vector<const char *> extensions) {
	std::vector<vk::ExtensionProperties> supported_extensions = physical_device.enumerateDeviceExtensionProperties();

	for (const char *required_extension : extensions) {
		bool found_extension = false;

		for (const vk::ExtensionProperties &supported_extension : supported_extensions) {
			if (strcmp(supported_extension.extensionName, required_extension) == 0) {
				found_extension = true;
				break;
			}
		}

		if (!found_extension) {
			return false;
		}
	}

	QueueFamilyIndices indices = QueueFamilyIndices::find_queue_families(physical_device, surface);

	return indices.is_complete();
}

void Device::pick_physical_device() {
	std::vector<vk::PhysicalDevice> physical_devices = engine.instance.enumeratePhysicalDevices();

	for (const vk::PhysicalDevice &device : physical_devices) {
		if (is_device_suitable(physical_device, engine.surface, engine.REQUIRED_EXTENSIONS)) {
			physical_device = device;
			indices = QueueFamilyIndices::find_queue_families(physical_device, engine.surface);
			return;
		}
	}

	throw std::runtime_error("no viable physical device found!");
}

void Device::init_logical_device() {
	std::vector<vk::DeviceQueueCreateInfo> queue_infos;
	std::set<uint32_t> unique_queue_families = {
		indices.graphics_family.value(), indices.present_family.value()
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

	vk::PhysicalDeviceFeatures required_features;
	required_features.setSamplerAnisotropy(vk::True);

	vk::DeviceCreateInfo device_info;
	device_info
			.setQueueCreateInfoCount(queue_infos.size())
			.setPQueueCreateInfos(queue_infos.data())
			.setPEnabledFeatures(&required_features)
			.setEnabledExtensionCount(engine.REQUIRED_EXTENSIONS.size())
			.setPEnabledExtensionNames(engine.REQUIRED_EXTENSIONS)
			.setPNext(&descriptor_indexing_features);

	if (engine.ENABLE_VALIDATION_LAYERS) {
		device_info
				.setEnabledLayerCount(engine.VALIDATION_LAYERS.size())
				.setPEnabledLayerNames(engine.VALIDATION_LAYERS);
	} else {
		device_info.setEnabledLayerCount(0);
	}

	logical_device = physical_device.createDevice(device_info);
}

Queues Device::get_device_queues() {
	Queues queues;
	queues.graphics = logical_device.getQueue(indices.graphics_family.value(), 0);
	queues.present = logical_device.getQueue(indices.present_family.value(), 0);
	return queues;
}

void Device::cleanup() {
	logical_device.destroy();
}
