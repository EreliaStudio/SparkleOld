#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{
	AbstractPipeline::Object::Storage::Configuration AbstractPipeline::_parseStorageBuffers(const std::string& p_vertexModuleCode)
	{
		AbstractPipeline::Object::Storage::Configuration result;

		return (result);
	}

	void AbstractPipeline::_loadAbstractPipeline(
		const std::string& p_vertexName, const std::string& p_vertexCode,
		const std::string& p_fragmentName, const std::string& p_fragmentCode)
	{
		_loadProgram(p_vertexName, p_vertexCode, p_fragmentName, p_fragmentCode);

		_storageConfiguration = _parseStorageBuffers(p_vertexCode);
	}


	AbstractPipeline::AbstractPipeline(const std::string& p_vertexName, const std::string& p_vertexCode, const std::string& p_fragmentName, const std::string& p_fragmentCode)
	{
		_loadAbstractPipeline(p_vertexName, p_vertexCode, p_fragmentName, p_fragmentCode);
	}

	AbstractPipeline::AbstractPipeline(const std::filesystem::path& p_vertexShaderPath, const std::filesystem::path& p_fragmentShaderPath)
	{
		std::string vertexName = p_vertexShaderPath.filename().string();
		std::string vertexCode = spk::getFileContentAsString(p_vertexShaderPath);

		std::string fragmentName = p_fragmentShaderPath.filename().string();
		std::string fragmentCode = spk::getFileContentAsString(p_fragmentShaderPath);

		_loadAbstractPipeline(vertexName, vertexCode, fragmentName, fragmentCode);
	}

	AbstractPipeline::Object AbstractPipeline::createObject()
	{
		return (Object(this, _storageConfiguration));
	}
}