#pragma once

#include "graphics/surface/spk_graphical_api_abstract_surface.hpp"
#include "spk_system_define.hpp"

namespace spk::GraphicalAPI
{
	class Surface : public spk::GraphicalAPI::AbstractSurface
	{
	private:
		HGLRC _hrc;
		HDC _hdc;
		PIXELFORMATDESCRIPTOR _pfd;

	public:
		Surface(spk::GraphicalAPI::AbstractFrame* p_frame);

        void resize();
		void clear();
		void render();
	};
}