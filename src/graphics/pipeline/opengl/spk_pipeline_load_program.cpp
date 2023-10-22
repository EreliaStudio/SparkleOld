#include "graphics/pipeline/spk_pipeline.hpp"
#include <regex>
namespace spk
{
	size_t findFirstBindingAvailable(const spk::ShaderLayout& p_shaderLayout);

	void compileShaderModule(GLuint p_shaderIndex, std::string p_shaderCode)
	{
		GLint result;
		GLint len;
		const char *content = p_shaderCode.c_str();

		result = GL_FALSE;

		glShaderSource(p_shaderIndex, 1, &content, NULL);
		glCompileShader(p_shaderIndex); //! X bytes in 1 blocks are possibly lost
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
		glLinkProgram(p_programID); //! X bytes in 1 blocks are possibly lost
		glGetProgramiv(p_programID, GL_LINK_STATUS, &result);
		if (result != GL_TRUE)
		{
			spk::throwException(L"Error while linking a program");
		}
		glDetachShader(p_programID, p_vertexID);
		glDetachShader(p_programID, p_fragmentID); //! X bytes in 1 blocks are possibly lost
	}

	/**
	 * @brief Converts shader code from Vulkan to OpenGL by replacing specific layout qualifiers.
	 *
	 * Specifically, this function replaces the Vulkan-specific `layout(push_constant)` qualifier
	 * with the OpenGL-compatible `layout(binding = n)` qualifier, where `n` is the first available
	 * binding slot in the provided ShaderLayout.
	 *
	 * @param p_shaderLayout The ShaderLayout used to find the first available binding slot.
	 * @param p_code The original Vulkan shader code.
	 * @return Modified shader code that is OpenGL compatible.
	 */
	std::string convertCodeToOpenGL(const spk::ShaderLayout& p_shaderLayout, const std::string& p_code)
	{
		std::string result = p_code;

		std::regex pushConstantRegex("layout\\(push_constant\\)");

		std::smatch matches;

		if (std::regex_search(result, matches, pushConstantRegex))
		{
			std::string replacement = "layout(binding = " + std::to_string(findFirstBindingAvailable(p_shaderLayout)) + ")";
			result = std::regex_replace(result, pushConstantRegex, replacement);
		}

		return (result);
	}
	
	void Pipeline::_loadProgram(const spk::ShaderLayout& p_shaderLayout)
	{
		_program = glCreateProgram(); //! X bytes in 1 blocks are possibly lost
		GLuint _vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint _fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		compileShaderModule(_vertexShaderID, convertCodeToOpenGL(p_shaderLayout, p_shaderLayout.vertexModule().code()));
		compileShaderModule(_fragmentShaderID, convertCodeToOpenGL(p_shaderLayout, p_shaderLayout.fragmentModule().code()));
		compileProgram(_program, _vertexShaderID, _fragmentShaderID);

		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
		activate();
	}
}