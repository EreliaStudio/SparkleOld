#pragma once

#include "graphics/surface/spk_graphical_api_abstract_surface.hpp"
#include "graphics/surface/vulkan/spk_graphical_api_device.hpp"

namespace spk::GraphicalAPI
{
	class Surface : public spk::GraphicalAPI::AbstractSurface
	{
	private:
		Device _device;

        void _onResize()
        {

        }

	public:
		Surface(const spk::Vector2UInt& p_size, Frame& p_frame) :
			spk::GraphicalAPI::AbstractSurface(p_size),
			_device(p_frame)
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