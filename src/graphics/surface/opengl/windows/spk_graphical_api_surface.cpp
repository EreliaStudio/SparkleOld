#include "graphics/surface/opengl/windows/spk_graphical_api_surface.hpp"
#include <GL/glew.h>
#include <GL/wglew.h>

namespace spk
{
	std::wstring glGetTypeString(const GLenum &type)
	{
		switch (type)
		{
		case GL_NO_ERROR:
			return (L"No error");
		case GL_INVALID_ENUM:
			return (L"Invalid enum");
		case GL_INVALID_VALUE:
			return (L"Invalid value");
		case GL_INVALID_OPERATION:
			return (L"Invalid operation");
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return (L"Invalid framebuffer operation");
		case GL_OUT_OF_MEMORY:
			return (L"Out of memory");
		case GL_STACK_UNDERFLOW:
			return (L"Stack underflow");
		case GL_STACK_OVERFLOW:
			return (L"Stack overflow");

		case GL_ELEMENT_ARRAY_BUFFER:
			return (L"GL_ELEMENT_ARRAY_BUFFER");
		case GL_ARRAY_BUFFER:
			return (L"GL_ARRAY_BUFFER");
		case GL_BYTE:
			return (L"GLbyte");
		case GL_UNSIGNED_BYTE:
			return (L"GLubyte");
		case GL_UNSIGNED_SHORT:
			return (L"GLushort");
		case GL_2_BYTES:
			return (L"GL_2_BYTES");
		case GL_3_BYTES:
			return (L"GL_3_BYTES");
		case GL_4_BYTES:
			return (L"GL_4_BYTES");

		case GL_FLOAT:
			return (L"float");
		case GL_FLOAT_VEC2:
			return (L"vec2");
		case GL_FLOAT_VEC3:
			return (L"vec3");
		case GL_FLOAT_VEC4:
			return (L"vec4");
		case GL_DOUBLE:
			return (L"double");
		case GL_DOUBLE_VEC2:
			return (L"dvec2");
		case GL_DOUBLE_VEC3:
			return (L"dvec3");
		case GL_DOUBLE_VEC4:
			return (L"dvec4");
		case GL_INT:
			return (L"int");
		case GL_INT_VEC2:
			return (L"ivec2");
		case GL_INT_VEC3:
			return (L"ivec3");
		case GL_INT_VEC4:
			return (L"ivec4");
		case GL_UNSIGNED_INT:
			return (L"unsigned int");
		case GL_UNSIGNED_INT_VEC2:
			return (L"uvec2");
		case GL_UNSIGNED_INT_VEC3:
			return (L"uvec3");
		case GL_UNSIGNED_INT_VEC4:
			return (L"uvec4");
		case GL_BOOL:
			return (L"bool");
		case GL_BOOL_VEC2:
			return (L"bvec2");
		case GL_BOOL_VEC3:
			return (L"bvec3");
		case GL_BOOL_VEC4:
			return (L"bvec4");
		case GL_FLOAT_MAT2:
			return (L"mat2");
		case GL_FLOAT_MAT3:
			return (L"mat3");
		case GL_FLOAT_MAT4:
			return (L"mat4");
		case GL_FLOAT_MAT2x3:
			return (L"mat2x3");
		case GL_FLOAT_MAT2x4:
			return (L"mat2x4");
		case GL_FLOAT_MAT3x2:
			return (L"mat3x2");
		case GL_FLOAT_MAT3x4:
			return (L"mat3x4");
		case GL_FLOAT_MAT4x2:
			return (L"mat4x2");
		case GL_FLOAT_MAT4x3:
			return (L"mat4x3");
		case GL_DOUBLE_MAT2:
			return (L"dmat2");
		case GL_DOUBLE_MAT3:
			return (L"dmat3");
		case GL_DOUBLE_MAT4:
			return (L"dmat4");
		case GL_DOUBLE_MAT2x3:
			return (L"dmat2x3");
		case GL_DOUBLE_MAT2x4:
			return (L"dmat2x4");
		case GL_DOUBLE_MAT3x2:
			return (L"dmat3x2");
		case GL_DOUBLE_MAT3x4:
			return (L"dmat3x4");
		case GL_DOUBLE_MAT4x2:
			return (L"dmat4x2");
		case GL_DOUBLE_MAT4x3:
			return (L"dmat4x3");
		case GL_SAMPLER_1D:
			return (L"sampler1D");
		case GL_SAMPLER_2D:
			return (L"sampler2D");
		case GL_SAMPLER_3D:
			return (L"sampler3D");
		case GL_SAMPLER_CUBE:
			return (L"samplerCube");
		case GL_SAMPLER_1D_SHADOW:
			return (L"sampler1DShadow");
		case GL_SAMPLER_2D_SHADOW:
			return (L"sampler2DShadow");
		case GL_SAMPLER_1D_ARRAY:
			return (L"sampler1DArray");
		case GL_SAMPLER_2D_ARRAY:
			return (L"sampler2DArray");
		case GL_SAMPLER_1D_ARRAY_SHADOW:
			return (L"sampler1DArrayShadow");
		case GL_SAMPLER_2D_ARRAY_SHADOW:
			return (L"sampler2DArrayShadow");
		case GL_SAMPLER_2D_MULTISAMPLE:
			return (L"sampler2DMS");
		case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
			return (L"sampler2DMSArray");
		case GL_SAMPLER_CUBE_SHADOW:
			return (L"samplerCubeShadow");
		case GL_SAMPLER_BUFFER:
			return (L"samplerBuffer");
		case GL_SAMPLER_2D_RECT:
			return (L"sampler2DRect");
		case GL_SAMPLER_2D_RECT_SHADOW:
			return (L"sampler2DRectShadow");
		case GL_INT_SAMPLER_1D:
			return (L"isampler1D");
		case GL_INT_SAMPLER_2D:
			return (L"isampler2D");
		case GL_INT_SAMPLER_3D:
			return (L"isampler3D");
		case GL_INT_SAMPLER_CUBE:
			return (L"isamplerCube");
		case GL_INT_SAMPLER_1D_ARRAY:
			return (L"isampler1DArray");
		case GL_INT_SAMPLER_2D_ARRAY:
			return (L"isampler2DArray");
		case GL_INT_SAMPLER_2D_MULTISAMPLE:
			return (L"isampler2DMS");
		case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
			return (L"isampler2DMSArray");
		case GL_INT_SAMPLER_BUFFER:
			return (L"isamplerBuffer");
		case GL_INT_SAMPLER_2D_RECT:
			return (L"isampler2DRect");
		case GL_UNSIGNED_INT_SAMPLER_1D:
			return (L"usampler1D");
		case GL_UNSIGNED_INT_SAMPLER_2D:
			return (L"usampler2D");
		case GL_UNSIGNED_INT_SAMPLER_3D:
			return (L"usampler3D");
		case GL_UNSIGNED_INT_SAMPLER_CUBE:
			return (L"usamplerCube");
		case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
			return (L"usampler2DArray");
		case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
			return (L"usampler2DArray");
		case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
			return (L"usampler2DMS");
		case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
			return (L"usampler2DMSArray");
		case GL_UNSIGNED_INT_SAMPLER_BUFFER:
			return (L"usamplerBuffer");
		case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
			return (L"usampler2DRect");
		case GL_IMAGE_1D:
			return (L"image1D");
		case GL_IMAGE_2D:
			return (L"image2D");
		case GL_IMAGE_3D:
			return (L"image3D");
		case GL_IMAGE_2D_RECT:
			return (L"image2DRect");
		case GL_IMAGE_CUBE:
			return (L"imageCube");
		case GL_IMAGE_BUFFER:
			return (L"imageBuffer");
		case GL_IMAGE_1D_ARRAY:
			return (L"image1DArray");
		case GL_IMAGE_2D_ARRAY:
			return (L"image2DArray");
		case GL_IMAGE_2D_MULTISAMPLE:
			return (L"image2DMS");
		case GL_IMAGE_2D_MULTISAMPLE_ARRAY:
			return (L"image2DMSArray");
		case GL_INT_IMAGE_1D:
			return (L"iimage1D");
		case GL_INT_IMAGE_2D:
			return (L"iimage2D");
		case GL_INT_IMAGE_3D:
			return (L"iimage3D");
		case GL_INT_IMAGE_2D_RECT:
			return (L"iimage2DRect");
		case GL_INT_IMAGE_CUBE:
			return (L"iimageCube");
		case GL_INT_IMAGE_BUFFER:
			return (L"iimageBuffer");
		case GL_INT_IMAGE_1D_ARRAY:
			return (L"iimage1DArray");
		case GL_INT_IMAGE_2D_ARRAY:
			return (L"iimage2DArray");
		case GL_INT_IMAGE_2D_MULTISAMPLE:
			return (L"iimage2DMS");
		case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
			return (L"iimage2DMSArray");
		case GL_UNSIGNED_INT_IMAGE_1D:
			return (L"uimage1D");
		case GL_UNSIGNED_INT_IMAGE_2D:
			return (L"uimage2D");
		case GL_UNSIGNED_INT_IMAGE_3D:
			return (L"uimage3D");
		case GL_UNSIGNED_INT_IMAGE_2D_RECT:
			return (L"uimage2DRect");
		case GL_UNSIGNED_INT_IMAGE_CUBE:
			return (L"uimageCube");
		case GL_UNSIGNED_INT_IMAGE_BUFFER:
			return (L"uimageBuffer");
		case GL_UNSIGNED_INT_IMAGE_1D_ARRAY:
			return (L"uimage1DArray");
		case GL_UNSIGNED_INT_IMAGE_2D_ARRAY:
			return (L"uimage2DArray");
		case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:
			return (L"uimage2DMS");
		case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
			return (L"uimage2DMSArray");
		case GL_UNSIGNED_INT_ATOMIC_COUNTER:
			return (L"atomic_uint");

		default:
			return (L"not a GLenum type");
		}
	}

	void GLAPIENTRY MessageOpenGLCallback(GLenum source,
										  GLenum type,
										  GLuint id,
										  GLenum severity,
										  GLsizei length,
										  const GLchar *message,
										  const void *userParam)
	{
		if (severity == GL_DEBUG_TYPE_ERROR || severity == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR || severity == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR ||
			severity == GL_DEBUG_TYPE_ERROR_ARB || severity == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB || severity == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)
			fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
	}
}

namespace spk::GraphicalAPI
{
	Surface::Surface(spk::GraphicalAPI::AbstractFrame *p_frame) : spk::GraphicalAPI::AbstractSurface(p_frame)
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
			0};

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

		wglSwapIntervalEXT(0);
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
		SwapBuffers(_hdc);
	}
}