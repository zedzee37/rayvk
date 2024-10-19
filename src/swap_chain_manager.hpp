#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

class SwapChainManager {
public:
	vk::SwapchainKHR swap_chain;
	vk::Format format;
	vk::Extent2D extent;
	std::vector<vk::Image> images;
	std::vector<vk::ImageView> image_views;
};
