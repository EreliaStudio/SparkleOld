#include "graphics/surface/vulkan/spk_graphical_api_renderer.hpp"

namespace spk::GraphicalAPI
{
	static vk::Extent2D toExtend2D(const spk::Vector2UInt& p_size)
	{
		return vk::Extent2D(p_size.x, p_size.y);
	}

	Renderer::Renderer(Frame& p_frame, Device& p_device)
		: _frame(p_frame), _device(p_device),
		_swapChain(std::make_unique<SwapChain>(_device, toExtend2D(_frame.size()))),
		_currentFrameIndex(0),
		_isFrameStarted(false)
	{
		_createCommandBuffers();
	}

	Renderer::~Renderer()
	{
		_freeCommandBuffers();
	}

	vk::CommandBuffer Renderer::beginFrame()
	{
		assert(_isFrameStarted == false && "Cannot begin new frame while the previous one is not ended.");

		vk::Result result = _swapChain->acquireNextImage(&_currentImageIndex);

		if (result == vk::Result::eErrorOutOfDateKHR)
		{
			_recreateSwapChain();
			return nullptr;
		}
		else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
			spk::throwException(L"Failed to acquire swap chain image");

		_isFrameStarted = true;

		auto commandBuffer = currentCommandBuffer();

		vk::CommandBufferBeginInfo beginInfo;
		if (commandBuffer.begin(&beginInfo) != vk::Result::eSuccess)
			spk::throwException(L"Failed to begin recording command buffer");

		return (commandBuffer);
	}

	void Renderer::beginSwapChainRenderPass(vk::CommandBuffer p_commandBuffer)
	{
		assert(_isFrameStarted == true && "Cannot begin swap chain render pass while frame is not started.");
		assert(p_commandBuffer == currentCommandBuffer() && "Cannot begin swap chain render pass with a command buffer that is not the current one.");

		std::array<vk::ClearValue, 2> clearValues(
			{
				vk::ClearColorValue(std::array<float, 4>{0.15f, 0.15f, 0.15f, 1.0f}),
				vk::ClearDepthStencilValue(1.0f, 0)
			}
		);

		vk::RenderPassBeginInfo renderPassInfo(
			_swapChain->renderPass(),
			_swapChain->frameBuffer(_currentImageIndex),
			vk::Rect2D({ 0, 0 }, _swapChain->extent()),
			static_cast<uint32_t>(clearValues.size()), clearValues.data()
		);

		p_commandBuffer.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);

		vk::Viewport viewport(0.0f, 0.0f,
			static_cast<float>(_swapChain->extent().width),
			static_cast<float>(_swapChain->extent().height),
			0.0f, 1.0f);
		p_commandBuffer.setViewport(0, 1, &viewport);

		vk::Rect2D scissor({ 0, 0 }, _swapChain->extent());
		p_commandBuffer.setScissor(0, 1, &scissor);
	}

	void Renderer::endSwapChainRenderPass(vk::CommandBuffer p_commandBuffer)
	{
		assert(_isFrameStarted == true && "Cannot end swap chain render pass while frame is not started.");
		assert(p_commandBuffer == currentCommandBuffer() && "Cannot end swap chain render pass with a command buffer that is not the current one.");

		p_commandBuffer.endRenderPass();
	}

	void Renderer::endFrame()
	{
		assert(_isFrameStarted == true && "Cannot end frame while it is not started.");

		vk::CommandBuffer commandBuffer = currentCommandBuffer();

		commandBuffer.end();

		vk::Result result = _swapChain->submitCommandBuffers(&commandBuffer, &_currentImageIndex);

		// if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR ||
		// 	_frame.wasResized() == true)
		if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
		{
			// _frame.resizeHandled();
			_recreateSwapChain();
		}
		else if (result != vk::Result::eSuccess)
			spk::throwException(L"Failed to present swap chain image");

		_isFrameStarted = false;
		_currentFrameIndex = (_currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void Renderer::_recreateSwapChain()
	{
		auto newSize(toExtend2D(_frame.size()));
		_device.device().waitIdle();

		std::shared_ptr<SwapChain> oldSwapChain = std::move(_swapChain);

		_swapChain = std::make_unique<SwapChain>(_device, newSize, oldSwapChain);

		if (oldSwapChain->isCompatible(*_swapChain) == false)
			spk::throwException(L"One of swap chain formats has changed"); //TODO/< Recreate a renderpass.
	}

	void Renderer::_createCommandBuffers()
	{
		_commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

		vk::CommandBufferAllocateInfo allocInfo(
			_device.commandPool(),
			vk::CommandBufferLevel::ePrimary,
			static_cast<uint32_t>(_commandBuffers.size())
		);

		if (_device.device().allocateCommandBuffers(&allocInfo, _commandBuffers.data()) != vk::Result::eSuccess)
			spk::throwException(L"Failed to allocate command buffers");
	}

	void Renderer::_freeCommandBuffers()
	{
		_device.device().freeCommandBuffers(_device.commandPool(),
			static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());
		_commandBuffers.clear();
	}
}