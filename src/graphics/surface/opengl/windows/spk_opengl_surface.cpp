#include "graphics/surface/opengl/spk_opengl_surface.hpp"
#include "graphics/spk_window.hpp"

namespace spk::OpenGL
{

	void Surface::_onViewportEdition()
	{
		glViewport(
				activeViewport()->anchor().x, 
				activeViewport()->anchor().y, 
				activeViewport()->size().x, 
				activeViewport()->size().y
			);
	}
		
	void Surface::_onScissorEdition()
	{
		glScissor(
				activeScissor()->anchor().x, 
				activeScissor()->anchor().y, 
				activeScissor()->size().x, 
				activeScissor()->size().y
			);
	}

	Surface::Surface(const std::wstring& p_title, const spk::Vector2UInt& p_size) :
		AbstractSurface(p_title, p_size)
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

		if (wglewIsSupported("WGL_ARB_create_context") == 1)
		{
			_hrc = wglCreateContextAttribsARB(_hdc, NULL, attributes);
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(tempOpenGLContext);
			wglMakeCurrent(_hdc, _hrc);
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glFrontFace(GL_CCW);

		glEnable(GL_DEPTH_TEST);
		glClearDepth(1.0f);
		glDepthFunc(GL_LESS);

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);

		glEnable(GL_SCISSOR_TEST);
	}

	Surface::~Surface()
	{
		if (_hrc)
        {
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(_hrc);
        }
        if (_hdc)
        {
            ReleaseDC(GetForegroundWindow(), _hdc);
        }
	}

	spk::Vector2 Surface::convertScreenToOpenGL(const spk::Vector2Int& p_screenPosition)
	{        
		return (spk::Vector2(
			2.0f * (static_cast<float>(p_screenPosition.x - _activeViewport->anchor().x) / _activeViewport->size().x) - 1.0f,
			2.0f * (static_cast<float>(p_screenPosition.y - _activeViewport->anchor().y) / _activeViewport->size().y) - 1.0f
			));
	}

	void Surface::clear()
	{
		glViewport(0, 0, _size.x, _size.y);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
	
	void Surface::resize(const spk::Vector2UInt& p_size)
	{
		_size = p_size;
	}

	void Surface::render()
	{
		SwapBuffers(_hdc);
	}
}