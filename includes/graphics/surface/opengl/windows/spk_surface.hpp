#pragma once

#include "graphics/surface/spk_abstract_surface.hpp"
#include "spk_system_define.hpp"

namespace spk
{
	class Surface : public spk::AbstractSurface
	{
	private:
		HGLRC _hrc;
		HDC _hdc;
		PIXELFORMATDESCRIPTOR _pfd;

	public:
		Surface(spk::AbstractFrame* p_frame);

        void resize();
		void clear();
		void render();
	};
}