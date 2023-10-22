#include "spk_system_define.hpp"
#include <string>

namespace spk
{
	/**
	 * @brief OpenGL debug callback function to handle and log runtime messages.
	 * 
	 * This function is invoked by the OpenGL runtime to report errors, warnings,
	 * or other diagnostic messages. It filters messages based on severity and
	 * logs them to the standard error stream.
	 * 
	 * @param p_source The source of the debug message.
	 * @param p_type The type of the debug message.
	 * @param p_id The ID of the message.
	 * @param p_severity The severity level of the message.
	 * @param p_length The length of the message string.
	 * @param p_message The actual debug message string.
	 * @param p_userParam A user-defined parameter.
	 */
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