#include "graphics/pipeline/opengl/spk_pipeline.hpp"
#include "spk_basic_functions.hpp"

namespace spk
{
	Pipeline::Pipeline(const std::filesystem::path& p_vertexShaderPath, const std::filesystem::path& p_fragmentShaderPath) :
		spk::AbstractPipeline()
	{
		loadFromFile(p_vertexShaderPath, p_fragmentShaderPath);
	}

	void Pipeline::activate()
	{
		glUseProgram(_program);
	}

	void Pipeline::deactivate()
	{
		glUseProgram(0);
	}

	void Pipeline::launch(const size_t& p_nbIndexes)
	{
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(p_nbIndexes), GL_UNSIGNED_INT, nullptr);
	}

	std::unique_ptr<Pipeline::UniformBlock> Pipeline::_createUniformBlock(const Pipeline::Configuration::UniformBlockLayout& p_layout)
	{
		return (std::make_unique<OpenGLUniformBlock>(this, p_layout));
	}

	std::unique_ptr<Pipeline::Object> Pipeline::createObject()
	{
		return (std::make_unique<OpenGLObject>(this));
	}

	const GLuint& Pipeline::program() const
	{
		return (_program);
	}

	void Pipeline::_loadProgram(
		const std::string& p_vertexName, const std::string& p_vertexCode,
		const std::string& p_fragmentName, const std::string& p_fragmentCode)
	{
		_program = glCreateProgram();
		GLuint _vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint _fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		spk::compileShaderModule(_vertexShaderID, p_vertexCode);
		spk::compileShaderModule(_fragmentShaderID, p_fragmentCode);
		spk::compileProgram(_program, _vertexShaderID, _fragmentShaderID);

		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
		activate();
	}
}