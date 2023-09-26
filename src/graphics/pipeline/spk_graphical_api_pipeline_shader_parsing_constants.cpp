#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

#include <regex>

namespace spk::GraphicalAPI
{
	void parseShaderStructures(AbstractPipeline::Object::Constants::Configuration& p_configuration, const std::string& p_shaderSource)
	{
		std::regex structurePattern(R"(struct\s+(\w+)\s+\{([\w\s\;\:\[\]]+)\}\;)");

		std::smatch match;
		std::sregex_iterator it(p_shaderSource.begin(), p_shaderSource.end(), structurePattern);
		std::sregex_iterator end;

		for (; it != end; ++it)
		{
			match = *it;
			std::string structName = match[1];
			std::string structBody = match[2];
			
			AbstractPipeline::Object::Constants::Configuration::Structure newStruct;
			size_t totalStructSize = 0;

			std::regex attributePattern(R"((\w+)\s+(\w+)(\[\d+\])?\;)");
			std::sregex_iterator attr_it(structBody.begin(), structBody.end(), attributePattern);
			std::sregex_iterator attr_end;

			for (; attr_it != attr_end; ++attr_it)
			{
				std::smatch attr_match = *attr_it;
				std::string typeName = attr_match[1];
				std::string varName = attr_match[2];

				if (p_configuration.structures.find(typeName) == p_configuration.structures.end())
					spk::throwException(L"Structure [" + spk::to_wstring(structName) + L"] - Type [" + spk::to_wstring(typeName) + L"] not found");

				AbstractPipeline::Object::Constants::Configuration::Structure::Attribute newAttribute = p_configuration.structures[typeName].attributes[""];
				newAttribute.offset = totalStructSize;
				newStruct.attributes[varName] = newAttribute;

				totalStructSize += (newAttribute.unitSize * newAttribute.format);
			}

			newStruct.size = totalStructSize;

			p_configuration.structures[structName] = newStruct;
		}
	}


	AbstractPipeline::Object::Constants::Configuration AbstractPipeline::_parseConstants(const std::string& p_vertexModuleCode, const std::string& p_fragmentModuleCode)
	{
		AbstractPipeline::Object::Constants::Configuration result;

		parseShaderStructures(result, p_vertexModuleCode);
		parseShaderStructures(result, p_fragmentModuleCode);

		return result;
	}
}