#include "graphics/surface/vulkan/spk_graphical_api_surface.hpp"
#include "graphics/pipeline/vulkan/spk_graphical_api_pipeline.hpp"

namespace spk::GraphicalAPI
{
	Surface::Surface(AbstractFrame* p_frame) :
		AbstractSurface(p_frame),
		_device(static_cast<Frame*>(p_frame)),
		_renderer(static_cast<Frame*>(p_frame), _device)
	{
		spk::GraphicalAPI::Pipeline::_linkedDevice = &_device;
		spk::GraphicalAPI::Pipeline::_linkedRenderer = &_renderer;
	}


	Surface::~Surface()
	{
		_device.device().waitIdle();
	}

	void Surface::resize()
	{

	}

	void Surface::clear()
	{

	}

	void Surface::render()
	{
		vk::CommandBuffer commandBuffer = _renderer.beginFrame();

		if (commandBuffer == vk::CommandBuffer(nullptr))
			return;
		_renderer.beginSwapChainRenderPass(commandBuffer);

		_renderer.endSwapChainRenderPass(commandBuffer);
		_renderer.endFrame();
	}
}