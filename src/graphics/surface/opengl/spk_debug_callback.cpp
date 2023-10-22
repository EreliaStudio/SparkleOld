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
	void GLAPIENTRY MessageOpenGLCallback(GLenum p_source,
										GLenum p_type,
										GLuint p_id,
										GLenum p_severity,
										GLsizei p_length,
										const GLchar *p_message,
										const void *p_userParam)
	{
		// Check if the message is of a high severity level
		if (p_severity == GL_DEBUG_TYPE_ERROR || p_severity == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR || p_severity == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR ||
			p_severity == GL_DEBUG_TYPE_ERROR_ARB || p_severity == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB || p_severity == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)
		{
			// Log the message to the standard error stream
			fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s",
					(p_type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), p_type, p_severity, p_message);
		}
	}
}