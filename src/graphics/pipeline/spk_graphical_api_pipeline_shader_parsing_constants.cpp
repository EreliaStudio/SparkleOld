#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

#include <regex>

namespace spk::GraphicalAPI
{
	void parseShaderStructures(AbstractPipeline::Object::Constants::Configuration& p_configuration, const std::string& p_shaderSource)
	{
		// Regular expression for parsing structures in the shader
		std::regex structurePattern(R"(struct\s+(\w+)\s+\{([\w\s\;\:\[\]]+)\}\;)");

		std::smatch match;
		std::sregex_iterator it(p_shaderSource.begin(), p_shaderSource.end(), structurePattern);
		std::sregex_iterator end;

		// Loop through each match for a structure
		for (; it != end; ++it)
		{
			match = *it;
			std::string structName = match[1];
			std::string structBody = match[2];
			
			// Initialize new structure and size accumulator
			AbstractPipeline::Object::Constants::Configuration::Structure newStruct;
			size_t totalStructSize = 0;

			// Regular expression for parsing attributes of a structure
			std::regex attributePattern(R"((\w+)\s+(\w+)(\[\d+\])?\;)");
			std::sregex_iterator attr_it(structBody.begin(), structBody.end(), attributePattern);
			std::sregex_iterator attr_end;

			// Loop through each attribute of the structure
			for (; attr_it != attr_end; ++attr_it)
			{
				std::smatch attr_match = *attr_it;
				std::string typeName = attr_match[1];
				std::string varName = attr_match[2];

				// Check if type of the attribute is recognized
				if (p_configuration.structures.find(typeName) == p_configuration.structures.end())
				{
					// Your own exception handling here
					// spk::throwException(...);
					continue;
				}

				// Copy attribute information from existing types
				AbstractPipeline::Object::Constants::Configuration::Structure::Attribute newAttribute = p_configuration.structures[typeName].attributes[""];
				newAttribute.offset = totalStructSize;
				// Update the new structure with the attribute
				newStruct.attributes[varName] = newAttribute;

				// Update the total size of the structure
				totalStructSize += (newAttribute.unitSize * newAttribute.format);
			}

			// Update the new structure's size field
			newStruct.size = totalStructSize;

			// Add the new structure to the configurations
			p_configuration.structures[structName] = newStruct;
		}
	}


	AbstractPipeline::Object::Constants::Configuration AbstractPipeline::_parseConstants(const std::string& p_vertexModuleCode, const std::string& p_fragmentModuleCode)
	{
		DEBUG_LINE();

		AbstractPipeline::Object::Constants::Configuration result;

		parseShaderStructures(result, p_vertexModuleCode);
		parseShaderStructures(result, p_fragmentModuleCode);

		spk::cout << result << std::endl;

		return (result);
	}
}