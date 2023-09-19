#pragma once

#include "graphics/surface/spk_graphical_api_abstract_surface.hpp"

namespace spk::GraphicalAPI
{
	class Surface : public spk::GraphicalAPI::AbstractSurface
	{
	private:
		HGLRC _hrc;
		HDC _hdc;
		PIXELFORMATDESCRIPTOR _pfd;

	public:
		Surface(spk::GraphicalAPI::AbstractFrame* p_frame) :
			spk::GraphicalAPI::AbstractSurface(p_frame)
		{
			_hdc = GetDC(GetForegroundWindow());

			memset(&_pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

			_pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			_pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
			_pfd.iPixelType = PFD_TYPE_RGBA;
			_pfd.cColorBits = 24;
			_pfd.cDepthBits = 32;
			_pfd.cStencilBits = 32;
			_pfd.iLayerType = PFD_MAIN_PLANE;

			int nPixelFormat = ChoosePixelFormat(_hdc, &_pfd);
			SetPixelFormat(_hdc, nPixelFormat, &_pfd);

			HGLRC tempOpenGLContext = wglCreateContext(_hdc);
			wglMakeCurrent(_hdc, tempOpenGLContext);

			glewInit();

			int attributes[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
				WGL_CONTEXT_MINOR_VERSION_ARB, 0,
				WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
				0
			};

			if (wglewIsSupported("WGL_ARB_create_context") == GL_FALSE)
			{
				_hrc = wglCreateContextAttribsARB(_hdc, NULL, attributes);
				wglMakeCurrent(NULL, NULL);
				wglDeleteContext(tempOpenGLContext);
				wglMakeCurrent(_hdc, _hrc);
			}

			if (glDebugMessageCallback)
			{
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(MessageOpenGLCallback, nullptr);
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

			wglSwapIntervalEXT(0);
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