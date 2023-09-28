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

		size_t newOffset = 0;
		for (const auto& field : p_configuration.storage.fields) {
			newOffset += field.attribute.size;
		}

		AbstractPipeline::Configuration::StorageLayout::Field newField(dataType, std::stoi(p_locationInput), newOffset);

		p_configuration.storage.fields.push_back(newField);

		p_configuration.storage.stride += dataType.size;
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
    }

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

		spk::cout << "New structure [" << spk::to_wstring(p_structureName) << L"] of size " << newData.size << std::endl;
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

	void parseShaderPushConstants(AbstractPipeline::Configuration& p_configuration, const std::string& p_shaderCode)
	{
	}

	void parseShaderUniforms(AbstractPipeline::Configuration& p_configuration, const std::string& p_shaderCode)
	{
	}

	AbstractPipeline::Configuration::Configuration() :
		dataTypes({
			{"int", Data(sizeof(int))},
			{"uint", Data(sizeof(unsigned int))},
			{"float", Data(sizeof(float))},
			
			{"ivec2", Data(sizeof(int) * 2)},
			{"uvec2", Data(sizeof(unsigned int) * 2)},
			{"vec2", Data(sizeof(float) * 2)},
			
			{"ivec3", Data(sizeof(int) * 3)},
			{"uvec3", Data(sizeof(unsigned int) * 3)},
			{"vec3", Data(sizeof(float) * 3)},
			
			{"ivec4", Data(sizeof(int) * 4)},
			{"uvec4", Data(sizeof(unsigned int) * 4)},
			{"vec4", Data(sizeof(float) * 4)},
			
			{"mat4", Data(sizeof(float) * 16)},
			
			{"sampler1D", Data(sizeof(int) * 1)},
			{"sampler2D", Data(sizeof(int) * 1)},
			{"sampler3D", Data(sizeof(int) * 1)},
			{"samplerCube", Data(sizeof(int) * 1)},
		})
	{

	}

	AbstractPipeline::Configuration::Configuration(const std::string& p_vertexCode, const std::string& p_fragmentCode) :
		Configuration()
	{
		parseShaderStorageBuffers(*this, p_vertexCode);

		parseShaderStructures(*this, p_vertexCode);
		parseShaderStructures(*this, p_fragmentCode);
	
		parseShaderPushConstants(*this, p_vertexCode);
	
		parseShaderUniforms(*this, p_vertexCode);
		parseShaderUniforms(*this, p_fragmentCode);
	}
}