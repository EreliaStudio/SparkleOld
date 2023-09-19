#pragma once

#include "graphics/surface/spk_graphical_api_abstract_surface.hpp"

namespace spk::GraphicalAPI
{
	class Surface : public spk::GraphicalAPI::AbstractSurface
	{
	private:
		GLXContext _glxContext;
		xcb_connection_t* _connection;
		xcb_window_t _window;

	public:
		Surface(spk::GraphicalAPI::AbstractFrame* p_frame) :
			spk::GraphicalAPI::AbstractSurface(p_frame)
		{
			_connection = p_frame->connection();
			_window = p_frame->window();

			GLint attributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
			XVisualInfo* vi = glXChooseVisual(_connection, DefaultScreen(_connection), attributes);

			_glxContext = glXCreateContext(_connection, vi, NULL, GL_TRUE);
			glXMakeCurrent(_connection, _window, _glxContext);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);

			glDisable(GL_DEPTH_TEST);
			glClearDepth(1.0f);
			glDepthFunc(GL_ALWAYS);

			glDisable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 0, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilMask(0xFF);

			glDisable(GL_SCISSOR_TEST);
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