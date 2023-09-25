#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{
	std::wostream& operator<<(std::wostream& p_out, const AbstractPipeline::Object::Storage::Configuration& p_config)
	{
		p_out << L"Stride: " << p_config.stride << std::endl;

		for (const auto& attribute : p_config.attributes) {
			p_out << L"Attribute [" << attribute.first << L"]:" << std::endl;
			p_out << L"    Location: " << attribute.second.location << std::endl;
			p_out << L"    Offset: " << attribute.second.offset << std::endl;
			p_out << L"    Unit: " << [&]() -> std::wstring {
				switch (attribute.second.type) {
				case AbstractPipeline::Object::Storage::Configuration::Attribute::Type::Float:
					return L"Float";
				case AbstractPipeline::Object::Storage::Configuration::Attribute::Type::Int:
					return L"Int";
				case AbstractPipeline::Object::Storage::Configuration::Attribute::Type::UInt:
					return L"UInt";
				}
				return L"Unknown"; // This should never happen
				}() << std::endl;
				p_out << L"    Format: " << attribute.second.format << " unit(s)" << std::endl;
		}
		return p_out;
	}

	void AbstractPipeline::_loadAbstractPipeline(
		const std::string& p_vertexName, const std::string& p_vertexCode,
		const std::string& p_fragmentName, const std::string& p_fragmentCode)
	{
		_loadProgram(p_vertexName, p_vertexCode, p_fragmentName, p_fragmentCode);

		_storageConfiguration = _parseStorageBuffers(p_vertexCode);
	}

	AbstractPipeline::AbstractPipeline()
	{

	}

	void AbstractPipeline::loadFromCode(const std::string& p_vertexCode, const std::string& p_fragmentCode)
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