#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"
#include <regex>

namespace spk::GraphicalAPI
{
	void parseShaderStructureBody(AbstractPipeline::Configuration& p_configuration, const std::string& p_structureName, const std::string& p_structureBody)
	{
		if (p_configuration.dataTypes.find(p_structureName) != p_configuration.dataTypes.end())
				spk::throwException(L"Data type [" + spk::to_wstring(p_structureName) + L"] already defined");

		AbstractPipeline::Configuration::Data newData;
		size_t currentOffset = 0;

		std::regex fieldRegex(R"((\w+)\s+(\w+)\s*;)");
		std::smatch match;

		std::string::const_iterator searchStart(p_structureBody.cbegin());

		while (std::regex_search(searchStart, p_structureBody.cend(), match, fieldRegex)) {
			std::string fieldType = match[1].str();
			std::string fieldName = match[2].str();

			auto it = p_configuration.dataTypes.find(fieldType);
			if (it == p_configuration.dataTypes.end())
				spk::throwException(L"Structure [" + spk::to_wstring(p_structureName) + L"] : Data type [" + spk::to_wstring(fieldType) + L"] doesn't exist");

			const AbstractPipeline::Configuration::Data& field = p_configuration.dataTypes[fieldType];

			newData.size += field.size;

			searchStart = match.suffix().first;
		}

		p_configuration.dataTypes[p_structureName] = newData;
	}

	void parseShaderStructures(AbstractPipeline::Configuration& p_configuration, const std::string& p_shaderCode) 
	{

		std::regex structureRegex(R"(struct\s+(\w+)\s*\{([^}]+)\}\s*;)");
		std::smatch match;

		std::string::const_iterator searchStart(p_shaderCode.cbegin());

		while (std::regex_search(searchStart, p_shaderCode.cend(), match, structureRegex)) 
		{
			std::string structName = match[1].str();
			std::string structBody = match[2].str();

			parseShaderStructureBody(p_configuration, structName, structBody);

			searchStart = match.suffix().first;
		}
	}
}