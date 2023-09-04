#include "graphics/surface/opengl/spk_opengl_surface.hpp"
#include <gl/glew.h>

namespace spk::OpenGL
{
	void GLAPIENTRY MessageOpenGLCallback(
		GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
		const GLchar* message,const void* userParam)
	{
		if (severity == GL_DEBUG_TYPE_ERROR || severity == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR || severity == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR ||
			severity == GL_DEBUG_TYPE_ERROR_ARB || severity == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB || severity == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)
			fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
	}

	Surface::Surface(const std::wstring& p_title, const spk::Vector2UInt& p_size) :
		AbstractSurface(p_title, p_size)
	{

	}

	void Surface::clear()
	{
		spk::cout << "Clearing OpenGL surface" << std::endl;
	}
	
	void Surface::resize(const spk::Vector2UInt& p_size)
	{
		spk::cout << "Resizing OpenGL surface" << std::endl;
	}

	void Surface::render()
	{
		spk::cout << "Render OpenGL surface" << std::endl;
	}
}