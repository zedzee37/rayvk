#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>

struct Image {
	static vk::ImageView createImageView(vk::Device device, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);
};

#endif
