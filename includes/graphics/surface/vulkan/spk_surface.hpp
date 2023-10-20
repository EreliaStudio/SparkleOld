#pragma once

#include "graphics/surface/spk_abstract_surface.hpp"
#include "graphics/surface/vulkan/spk_device.hpp"
#include "graphics/surface/vulkan/spk_renderer.hpp"

namespace spk
{
	class Surface : public spk::AbstractSurface
	{
	private:
		Device _device;
		Renderer _renderer;

	public:
		Surface(AbstractFrame* p_frame);

		~Surface();

		void resize();

		void clear();
		void render();
	};
}