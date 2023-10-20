#pragma once

#include "graphics/surface/spk_abstract_surface.hpp"
#include "graphics/spk_frame.hpp"
#include <GL/gl.h>
#include <GL/glx.h>

namespace spk
{
	class Surface : public spk::AbstractSurface
	{
	private:
		GLXContext _glxContext;

		Display* _display;

		xcb_connection_t* _connection;
		xcb_window_t _window;

	public:
		Surface(spk::Frame* p_frame);

        void resize();

		void clear();

		void render();
	};
}