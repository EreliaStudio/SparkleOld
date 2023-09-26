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
		Surface(AbstractFrame* p_frame);

		~Surface();

		void resize();

		void clear();
		void render();
	};
}