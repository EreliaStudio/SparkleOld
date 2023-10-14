#include "graphics/pipeline/spk_pipeline.hpp"

namespace spk
{
	void compileShaderModule(GLuint p_shaderIndex, std::string p_shaderCode)
	{
		GLint result;
		GLint len;
		const char *content = p_shaderCode.c_str();

		result = GL_FALSE;

		glShaderSource(p_shaderIndex, 1, &content, NULL);
		glCompileShader(p_shaderIndex);
		glGetShaderiv(p_shaderIndex, GL_COMPILE_STATUS, &result);

		if (result != GL_TRUE)
		{
			glGetShaderiv(p_shaderIndex, GL_INFO_LOG_LENGTH, &len);
			char *errorMsg = new char[len + 1];
			glGetShaderInfoLog(p_shaderIndex, len, NULL, errorMsg);
			spk::throwException(L"Error while compiling a shader :\n" + spk::to_wstring(errorMsg));
		}
	}

	void compileProgram(GLuint p_programID, GLuint p_vertexID, GLuint p_fragmentID)
	{
		GLint result;

		result = GL_FALSE;
		glAttachShader(p_programID, p_vertexID);
		glAttachShader(p_programID, p_fragmentID);
		glLinkProgram(p_programID);
		glGetProgramiv(p_programID, GL_LINK_STATUS, &result);
		if (result != GL_TRUE)
		{
			spk::throwException(L"Error while linking a program");
		}
		glDetachShader(p_programID, p_vertexID);
		glDetachShader(p_programID, p_fragmentID);
	}
	
	void Pipeline::_loadProgram(const spk::ShaderLayout& p_shaderLayout)
	{

	}
}