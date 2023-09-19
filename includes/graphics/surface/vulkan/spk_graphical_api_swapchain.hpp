#pragma once

#include "graphics/surface/vulkan/spk_graphical_api_device.hpp"

#include <string>
#include <vector>
#include <memory>

namespace spk::GraphicalAPI
{
	class SwapChain
	{
	public:
		static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

		SwapChain(Device& p_deviceRef, vk::Extent2D p_windowExtent);
		SwapChain(Device& p_deviceRef, vk::Extent2D p_windowExtent, std::shared_ptr<SwapChain> p_previous);
		~SwapChain();

		SwapChain(const SwapChain&) = delete;
		void operator=(const SwapChain&) = delete;

		vk::Framebuffer frameBuffer(int p_index) { return _framebuffers[p_index]; }
		vk::RenderPass renderPass() { return _renderPass; }
		vk::ImageView imageView(int p_index) { return _imageViews[p_index]; }
		size_t imageCount() { return _images.size(); }
		vk::Format imageFormat() { return _imageFormat; }
		vk::Extent2D extent() { return _extent; }
		uint32_t width() { return _extent.width; }
		uint32_t height() { return _extent.height; }

		float extentAspectRatio() const { return static_cast<float>(_extent.width) / static_cast<float>(_extent.height); }
		vk::Format findDepthFormat();

		vk::Result acquireNextImage(uint32_t* imageIndex);
		vk::Result submitCommandBuffers(const vk::CommandBuffer* buffers, uint32_t* imageIndex);

		bool isCompatible(const SwapChain& swapChain) const
		{
			return (_imageFormat == swapChain._imageFormat &&
				_depthFormat == swapChain._depthFormat);
		}

	private:
		void _init();
		void _createSwapChain();
		void _createImageViews();
		void _createDepthResources();
		void _createRenderPass();
		void _createFramebuffers();
		void _createSyncObjects();

		// Helper functions
		vk::SurfaceFormatKHR _chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
		vk::PresentModeKHR _chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
		vk::Extent2D _chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

		vk::Format _imageFormat;
		vk::Format _depthFormat;
		vk::Extent2D _extent;

		std::vector<vk::Framebuffer> _framebuffers;
		vk::RenderPass _renderPass;

		std::vector<vk::Image> _depthImages;
		std::vector<vk::DeviceMemory> _depthImageMemories;
		std::vector<vk::ImageView> _depthImageViews;
		std::vector<vk::Image> _images;
		std::vector<vk::ImageView> _imageViews;

		Device& _device;
		vk::Extent2D _windowExtent;

		vk::SwapchainKHR _swapChain;
		std::shared_ptr<SwapChain> _oldSwapChain;

		std::vector<vk::Semaphore> _imageAvailableSemaphores;
		std::vector<vk::Semaphore> _renderFinishedSemaphores;
		std::vector<vk::Fence> _inFlightFences;
		std::vector<vk::Fence> _imagesInFlight;
		size_t _currentFrame = 0;
	};
}
