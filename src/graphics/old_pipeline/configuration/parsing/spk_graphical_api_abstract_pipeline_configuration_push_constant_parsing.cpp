#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"
#include <regex>

namespace spk::GraphicalAPI
{
	void ShaderLayout::parseLayoutPushConstantInstruction_blockType(const std::string &p_instruction)
	{
		std::regex blockTypeRegex(R"(layout\s*\(push_constant\)\s*uniform\s*(\w+))");
		std::smatch matchResults;

		if (std::regex_search(p_instruction, matchResults, blockTypeRegex)) {
			constants.type = spk::to_wstring(matchResults[1]);
		} else {
			spk::throwException(L"Block type not found for instruction [" + spk::to_wstring(p_instruction) + L"]");
		}
	}
	
	void ShaderLayout::parseLayoutPushConstantInstruction_content(const std::string &p_instruction)
	{
		std::regex contentRegex(R"(\{\s*(\w+)\s+(\w+)\s*;\s*\})");
		auto contentBegin = std::sregex_iterator(p_instruction.begin(), p_instruction.end(), contentRegex);
		auto contentEnd = std::sregex_iterator();

		constants.stride = 0;
		for (std::sregex_iterator i = contentBegin; i != contentEnd; ++i) {
			PushConstantLayout::Field newField;

			std::smatch contentMatch = *i;
			std::string dataType = contentMatch[1];
			std::string variableName = contentMatch[2];

			newField.type = spk::to_wstring(dataType);
			newField.name = spk::to_wstring(variableName);
			newField.offset = constants.stride;
			if (dataTypes.find(dataType) != dataTypes.end())
			{
				newField.attribute = dataTypes[dataType];
			}
			else
			{
				spk::throwException(L"Invalid data type [" + spk::to_wstring(dataType) + L"] for instruction [" + spk::to_wstring(p_instruction) + L"]");
			}
			constants.fields.push_back(newField);
			constants.stride += newField.attribute.format * newField.attribute.size;
		}
	}
	
	void ShaderLayout::parseLayoutPushConstantInstruction_instanceName(const std::string &p_instruction)
	{
		std::regex instanceNameRegex(R"(\}\s*(\w+)\s*)");
        std::smatch matchResults;

        if (std::regex_search(p_instruction, matchResults, instanceNameRegex))
		{
			constants.name = spk::to_wstring(matchResults[1]);
        }
		else
		{
			spk::throwException(L"Instance name not found for instruction [" + spk::to_wstring(p_instruction) + L"]");
		}
	}

	void ShaderLayout::parseLayoutPushConstantInstruction(const std::string &p_instruction)
	{
		parseLayoutPushConstantInstruction_blockType(p_instruction);
		parseLayoutPushConstantInstruction_content(p_instruction);
		parseLayoutPushConstantInstruction_instanceName(p_instruction);
	}
}