#pragma once

#include "graphics/surface/vulkan/spk_graphical_api_device.hpp"
#include "graphics/surface/vulkan/spk_graphical_api_swapchain.hpp"
#include "graphics/spk_graphical_api_frame.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace spk::GraphicalAPI
{
	/**
	 * @class Renderer
	 * @brief Class that owns the swapchain, command buffers and that can draw.
	 */
	class Renderer
	{
	public:
		Renderer(Frame* p_frame, Device& p_device);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		bool isFrameStarted() const { return _isFrameStarted; }

		vk::CommandBuffer currentCommandBuffer() const
		{
			assert(_isFrameStarted == true && "Cannot get command buffer when frame is not started.");

			return _commandBuffers[_currentFrameIndex];
		}

		int currentFrameIndex() const
		{
			assert(_isFrameStarted == true && "Cannot get frame index when frame is not started.");

			return _currentFrameIndex;
		}

		vk::CommandBuffer beginFrame();
		void beginSwapChainRenderPass(vk::CommandBuffer p_commandBuffer); //?/ Maybe the mapping to the correct command buffer can be automatically done.
		float aspectRatio() const { return _swapChain->extentAspectRatio(); }
		void endSwapChainRenderPass(vk::CommandBuffer p_commandBuffer);
		void endFrame();

		vk::RenderPass renderPass() const { return _swapChain->renderPass(); }

	private:
		void _createCommandBuffers();
		void _freeCommandBuffers();
		void _recreateSwapChain();

		Frame* _frame;
		Device& _device;
		std::unique_ptr<SwapChain> _swapChain;
		std::vector<vk::CommandBuffer> _commandBuffers;

		uint32_t _currentImageIndex;
		int _currentFrameIndex;
		bool _isFrameStarted;
	};
}
