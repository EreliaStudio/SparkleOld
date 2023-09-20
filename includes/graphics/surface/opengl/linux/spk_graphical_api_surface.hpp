#pragma once

#include "graphics/surface/spk_graphical_api_abstract_surface.hpp"
#include "graphics/spk_graphical_api_frame.hpp"
#include <GL/gl.h>
#include <GL/glx.h>

namespace spk::GraphicalAPI
{
	class Surface : public spk::GraphicalAPI::AbstractSurface
	{
	private:
		GLXContext _glxContext;

		Display* _display;

		xcb_connection_t* _connection;
		xcb_window_t _window;

	public:
		Surface(spk::GraphicalAPI::Frame* p_frame);

        void resize();

		void clear();

		void render();
	};
}