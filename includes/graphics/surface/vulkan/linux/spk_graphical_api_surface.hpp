#pragma once

#include "graphics/surface/spk_graphical_api_abstract_surface.hpp"
#include "graphics/surface/vulkan/spk_graphical_api_device.hpp"
#include "graphics/surface/vulkan/spk_graphical_api_renderer.hpp"

namespace spk::GraphicalAPI
{
	class Surface : public spk::GraphicalAPI::AbstractSurface
	{
	private:
		Device _device;
		Renderer _renderer;

	public:
		Surface(const spk::Vector2UInt& p_size, Frame& p_frame) :
			spk::GraphicalAPI::AbstractSurface(p_size),
			_device(p_frame),
			_renderer(p_frame, _device)
		{

		}

		~Surface()
		{
			_device.device().waitIdle();
		}

		void resize()
		{

		}

		void clear()
		{

		}

		void render()
		{
			vk::CommandBuffer commandBuffer = _renderer.beginFrame();

			if (commandBuffer == vk::CommandBuffer(nullptr))
				return;
			_renderer.beginSwapChainRenderPass(commandBuffer);

			_renderer.endSwapChainRenderPass(commandBuffer);
			_renderer.endFrame();
		}
	};
}