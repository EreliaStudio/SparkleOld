#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"
#include <regex>

namespace spk::GraphicalAPI
{
	void AbstractPipeline::Configuration::parseLayoutUniformInstruction_layoutInfo(UniformBlockLayout &p_uniformBlockToFill, const std::string &p_instruction)
	{
		std::regex layoutRegex("layout\\s*\\(([^\\)]+)\\)");
		std::smatch match;

		if (std::regex_search(p_instruction, match, layoutRegex))
		{
			std::string layoutParams = match[1];
			std::regex paramRegex("\\s*(\\w+)\\s*=\\s*([^,]+)");
			std::smatch paramMatch;
			std::string::const_iterator searchStart(layoutParams.cbegin());
			int index = 0;

			while (std::regex_search(searchStart, layoutParams.cend(), paramMatch, paramRegex))
			{
				std::string paramName = paramMatch[1].str();
				std::string paramValue = paramMatch[2].str();

				if (paramName == "set")
				{
					p_uniformBlockToFill.set = std::stoi(paramValue);
				}
				else if (paramName == "binding")
				{
					p_uniformBlockToFill.binding = std::stoi(paramValue);
				}
				else
				{
					spk::throwException(L"Unrecognized layout type [" + spk::to_wstring(paramName) + L"]");
				}

				searchStart = paramMatch.suffix().first;
				index++;
			}
		}
	}

	void AbstractPipeline::Configuration::parseLayoutUniformInstruction_blockType(UniformBlockLayout &p_uniformBlockToFill, const std::string &p_instruction)
	{
		std::regex blockTypeRegex("uniform\\s+(\\w+)\\s*\\{");
		std::smatch blockTypeMatch;

		if (std::regex_search(p_instruction, blockTypeMatch, blockTypeRegex))
		{
			std::string blockType = blockTypeMatch[1].str();
			p_uniformBlockToFill.type = spk::to_wstring(blockType);
		}
	}

	void AbstractPipeline::Configuration::parseLayoutUniformInstruction_content(UniformBlockLayout &p_uniformBlockToFill, const std::string &p_instruction)
	{
		std::regex fieldRegex("(\\w+)\\s+(\\w+);");
		std::smatch fieldMatch;
		std::string::const_iterator searchStart(p_instruction.cbegin());

		p_uniformBlockToFill.stride = 0;

		while (std::regex_search(searchStart, p_instruction.cend(), fieldMatch, fieldRegex))
		{
			std::string glslType = fieldMatch[1].str();
			std::string fieldName = fieldMatch[2].str();

			if (dataTypes.find(glslType) != dataTypes.end())
			{
				Data fieldData = dataTypes[glslType];

				UniformBlockLayout::Field field;
				field.name = spk::to_wstring(fieldName);
				field.attribute = fieldData;
				field.offset = p_uniformBlockToFill.stride;

				p_uniformBlockToFill.fields.push_back(field);

				p_uniformBlockToFill.stride += fieldData.format * fieldData.size;
			}
			else
			{
				spk::throwException(L"Unrecognized GLSL type [" + spk::to_wstring(glslType) + L"]");
			}

			searchStart = fieldMatch.suffix().first;
		}
	}

	void AbstractPipeline::Configuration::parseLayoutUniformInstruction_instanceName(UniformBlockLayout &p_uniformBlockToFill, const std::string &p_instruction)
	{
		std::regex instanceNameRegex("\\}\\s*(\\w+)");
		std::smatch instanceNameMatch;

		if (std::regex_search(p_instruction, instanceNameMatch, instanceNameRegex))
		{
			std::string instanceName = instanceNameMatch[1].str();
			p_uniformBlockToFill.name = spk::to_wstring(instanceName);
		}
	}

	void AbstractPipeline::Configuration::parseLayoutUniformInstruction(const std::string &p_instruction)
	{
		UniformBlockLayout newUniformBlock;

		parseLayoutUniformInstruction_layoutInfo(newUniformBlock, p_instruction);
		parseLayoutUniformInstruction_blockType(newUniformBlock, p_instruction);
		parseLayoutUniformInstruction_content(newUniformBlock, p_instruction);
		parseLayoutUniformInstruction_instanceName(newUniformBlock, p_instruction);

		uniformBlocks.push_back(newUniformBlock);
	}


}