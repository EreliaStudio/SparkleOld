#include "graphics/surface/opengl/linux/spk_surface.hpp"

#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>

namespace spk
{
	void GLAPIENTRY MessageOpenGLCallback(GLenum source,
										  GLenum type,
										  GLuint id,
										  GLenum severity,
										  GLsizei length,
										  const GLchar *message,
										  const void *userParam);

	Surface::Surface(spk::Frame* p_frame) :
		spk::AbstractSurface(p_frame)
	{
		_display = p_frame->display();
		_connection = p_frame->connection();
		_window = p_frame->window();
		
		GLint attributes[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
		GLX_CONTEXT_MINOR_VERSION_ARB, 0,
		GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		XVisualInfo* vi = glXChooseVisual(_display, p_frame->defaultScreen(), attributes);
		if (vi == nullptr)
			spk::throwException(L"No appropriate visual found");

		_glxContext = glXCreateContext(_display, vi, NULL, GL_TRUE);
		glXMakeCurrent(_display, _window, _glxContext);

		GLenum p_glewInitResult = glewInit();

		if (glDebugMessageCallback)
		{
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(spk::MessageOpenGLCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}

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

		free(vi);
	}

	void Surface::resize()
	{
	}

	void Surface::clear()
	{
		glViewport(0, 0, _frame->size().x, _frame->size().y);
		glScissor(0, 0, _frame->size().x, _frame->size().y);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Surface::render()
	{
		glXSwapBuffers(_display, _window);
	}
}