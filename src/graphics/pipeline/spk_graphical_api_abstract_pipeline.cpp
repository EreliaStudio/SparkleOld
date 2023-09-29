#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{
	void convertVulkanToOpenglCompatibleGLSLCode(std::string& p_vertexCode, std::string& p_fragmentCode);

	void AbstractPipeline::_loadAbstractPipeline(
		const std::string& p_vertexName, std::string& p_vertexCode,
		const std::string& p_fragmentName, std::string& p_fragmentCode)
	{
		_configuration = AbstractPipeline::Configuration(p_vertexCode, p_fragmentCode);

#if GRAPHICAL_API == 0
		convertVulkanToOpenglCompatibleGLSLCode(p_vertexCode, p_fragmentCode);
#endif

		_loadProgram(p_vertexName, p_vertexCode, p_fragmentName, p_fragmentCode);
	}

	AbstractPipeline::AbstractPipeline()
	{

	}
	
	const AbstractPipeline::Configuration& AbstractPipeline::configuration() const
	{
		return (_configuration);
	}

	void AbstractPipeline::loadFromCode(std::string p_vertexCode, std::string p_fragmentCode)
	{
		_loadAbstractPipeline("VertexModule", p_vertexCode, "FragmentModule", p_fragmentCode);
	}

	void AbstractPipeline::loadFromFile(const std::filesystem::path& p_vertexShaderPath, const std::filesystem::path& p_fragmentShaderPath)
	{
		std::string vertexName = p_vertexShaderPath.filename().string();
		std::string vertexCode = spk::getFileContentAsString(p_vertexShaderPath);

		std::string fragmentName = p_fragmentShaderPath.filename().string();
		std::string fragmentCode = spk::getFileContentAsString(p_fragmentShaderPath);

		_loadAbstractPipeline(vertexName, vertexCode, fragmentName, fragmentCode);
	}
}