#include "spk_system_define.hpp"
#include <string>

namespace spk
{
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