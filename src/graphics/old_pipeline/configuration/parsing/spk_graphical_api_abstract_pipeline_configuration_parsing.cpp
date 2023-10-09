#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"
#include <regex>

namespace spk::GraphicalAPI
{
	bool isValidInstruction(const std::string& p_shaderCode, const size_t& p_startingPosition)
	{
		if (p_shaderCode.substr(p_startingPosition, 6) == "layout" || p_shaderCode.substr(p_startingPosition, 6) == "struct")
			return (true);
		return (false);
	}

	std::vector<std::string> splitShaderCodeIntoInstruction(const std::string &p_shaderCode)
	{
		std::vector<std::string> result;
		size_t startingPosition = 0;
		bool validInstruction = isValidInstruction(p_shaderCode, startingPosition);
		int braceLevel = 0;

		for (size_t i = 0; i < p_shaderCode.size(); ++i)
		{
			if (p_shaderCode[i] == '{')
			{
				braceLevel++;
			}
			else if (p_shaderCode[i] == '}')
			{
				braceLevel--;
				if (validInstruction == false)
				{
					startingPosition = i + 1;
					validInstruction = isValidInstruction(p_shaderCode, startingPosition);
				}
			}
			else if (p_shaderCode[i] == ';' && braceLevel <= 0)
			{
				result.push_back(p_shaderCode.substr(startingPosition, i - startingPosition));
				startingPosition = i + 1;
				validInstruction = isValidInstruction(p_shaderCode, startingPosition);
			}
		}

		return result;
	}

	bool ShaderLayout::isUniformLayoutInstruction(const std::string &p_instruction)
	{
		std::istringstream iss(p_instruction);
		std::string word;

		while (iss >> word)
		{
			if (word == "in" || word == "out")
			{
				return false;
			}
		}

		return true;
	}

	bool ShaderLayout::isLayoutBufferInInstruction(const std::string &p_instruction)
	{
		std::istringstream iss(p_instruction);
		std::string word;

		while (iss >> word)
		{
			if (word == "in")
			{
				return true;
			}
		}

		return false;
	}

	bool ShaderLayout::isPushConstantUniformInstruction(const std::string &p_instruction)
	{
		std::regex layoutRegex("layout\\s*\\(([^\\)]+)\\)");
		std::smatch match;

		if (std::regex_search(p_instruction, match, layoutRegex))
		{
			std::string layoutContents = match[1].str();
			return layoutContents.find("push_constant") != std::string::npos;
		}

		return false;
	}
	
	void ShaderLayout::parseLayoutInstruction(const std::string &p_instruction, const bool& p_bufferParsingMode)
	{
		if (isUniformLayoutInstruction(p_instruction) == true)
		{
			if (isPushConstantUniformInstruction(p_instruction) == true)
			{
				parseLayoutPushConstantInstruction(p_instruction);
			}
			else
			{
				parseLayoutUniformInstruction(p_instruction);
			}
		}
		else if (isLayoutBufferInInstruction(p_instruction) == true && p_bufferParsingMode == true)
		{
			parseLayoutBufferInstruction(p_instruction);
		}
	}

	void ShaderLayout::parseShaderInstruction(const std::string &p_instruction, const bool& p_bufferParsingMode)
	{
		if (p_instruction.find("layout") == 0)
		{
			parseLayoutInstruction(p_instruction, p_bufferParsingMode);
		}
		else if (p_instruction.find("struct") == 0)
		{
			parseStructInstruction(p_instruction);
		}
	}

	void ShaderLayout::parseShaderCode(const std::string &p_shaderCode, const bool& p_bufferParsingMode)
	{
		std::vector<std::string> instructions = splitShaderCodeIntoInstruction(p_shaderCode);

		for (size_t i = 0; i < instructions.size(); i++)
		{
			parseShaderInstruction(instructions[i], p_bufferParsingMode);
		}
	}
}