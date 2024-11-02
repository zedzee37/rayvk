#include "image.hpp"
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_structs.hpp>

vk::ImageView Image::createImageView(vk::Device device, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags) {
	vk::ImageSubresourceRange subResourceRange{};
	subResourceRange
			.setAspectMask(aspectFlags)
			.setBaseMipLevel(0)
			.setLevelCount(1)
			.setBaseArrayLayer(0)
			.setLayerCount(0);

	vk::ImageViewCreateInfo viewInfo{};
	viewInfo
			.setImage(image)
			.setViewType(vk::ImageViewType::e2D)
			.setFormat(format)
			.setSubresourceRange(subResourceRange);

	vk::ImageView imageView = device.createImageView(viewInfo);
	return imageView;
}
