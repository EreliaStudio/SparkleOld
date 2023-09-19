#pragma once

#include "graphics/surface/spk_graphical_api_abstract_surface.hpp"
#include "graphics/surface/vulkan/spk_graphical_api_device.hpp"

namespace spk::GraphicalAPI
{
	class Surface : public spk::GraphicalAPI::AbstractSurface
	{
	private:
		Device _device;

	public:
		Surface(const spk::GraphicalAPI::AbstractFrame* p_frame) :
			spk::GraphicalAPI::AbstractSurface(p_frame),
			_device(*p_frame)
		{

		}

        void resize()
        {

        }

		void clear()
		{

		}

		void render()
		{

		}
	};
}