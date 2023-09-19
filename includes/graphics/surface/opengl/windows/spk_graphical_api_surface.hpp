#pragma once

#include "graphics/surface/spk_graphical_api_abstract_surface.hpp"

namespace spk::GraphicalAPI
{
	class Surface : public spk::GraphicalAPI::AbstractSurface
	{
	private:

	public:
		Surface(const spk::GraphicalAPI::AbstractFrame* p_frame) :
			spk::GraphicalAPI::AbstractSurface(p_frame)
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