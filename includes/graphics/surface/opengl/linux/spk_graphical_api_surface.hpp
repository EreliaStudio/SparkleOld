#pragma once

#include "graphics/surface/spk_graphical_api_abstract_surface.hpp"

namespace spk::GraphicalAPI
{
	class Surface : public spk::GraphicalAPI::AbstractSurface
	{
	private:

        void _onResize()
        {

        }

	public:
		Surface(const spk::Vector2UInt& p_size) :
			spk::GraphicalAPI::AbstractSurface(p_size)
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