#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"
#include <regex>

namespace spk::GraphicalAPI
{
	void loadShaderStorageBufferUnit(AbstractPipeline::Configuration& p_configuration, const std::string& p_locationInput, const std::string& p_typeInput)
	{
		auto it = p_configuration.dataTypes.find(p_typeInput);
		if (it == p_configuration.dataTypes.end()) {
			spk::throwException(L"Invalid data type: " + spk::to_wstring(p_typeInput));
		}
		AbstractPipeline::Configuration::Data dataType = it->second;

		AbstractPipeline::Configuration::StorageLayout::Field newField(dataType, std::stoi(p_locationInput), p_configuration.storage.stride);

		p_configuration.storage.fields.push_back(newField);

		p_configuration.storage.stride += dataType.size * dataType.format;
	}

	void parseShaderStorageBuffers(AbstractPipeline::Configuration& p_configuration, const std::string& p_shaderCode)
    {
        std::regex bufferRegex(R"(^.*layout\s*\(\s*location\s*=\s*(\d+)\s*\)\s*in\s+(\w+)\s+\w+\s*;.*$)", std::regex_constants::ECMAScript | std::regex_constants::icase);
        std::regex inVariableRegex(R"(^.*\bin\b.*$)", std::regex_constants::ECMAScript | std::regex_constants::icase);

        std::smatch match;
        
        std::string::const_iterator searchStart = p_shaderCode.cbegin();

        while (std::regex_search(searchStart, p_shaderCode.cend(), match, inVariableRegex)) 
        {
            std::string matchedLine = match[0].str();
            std::smatch bufferMatch;

            if (!std::regex_search(matchedLine, bufferMatch, bufferRegex)) 
                spk::throwException(L"Input variable in line [" + spk::to_wstring(matchedLine) + L"] lacks a layout location");

			std::string locationValue = bufferMatch[1].str();
			std::string typeValue = bufferMatch[2].str();
			            
			loadShaderStorageBufferUnit(p_configuration, locationValue, typeValue);

            searchStart = match.suffix().first;
        }

		for (auto& bufferUnit : p_configuration.storage.fields)
		{
			bufferUnit.offset = p_configuration.storage.stride - bufferUnit.offset - bufferUnit.attribute.format * bufferUnit.attribute.size;
		}
    }
}