#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"
#include <regex>

namespace spk::GraphicalAPI
{
	AbstractPipeline::Configuration::Configuration() : dataTypes({
														   {"int", Data(1, Data::Type::Int)},
														   {"uint", Data(1, Data::Type::UInt)},
														   {"float", Data(1, Data::Type::Float)},

														   {"ivec2", Data(2, Data::Type::Int)},
														   {"uvec2", Data(2, Data::Type::UInt)},
														   {"vec2", Data(2, Data::Type::Float)},

														   {"ivec3", Data(3, Data::Type::Int)},
														   {"uvec3", Data(3, Data::Type::UInt)},
														   {"vec3", Data(3, Data::Type::Float)},

														   {"ivec4", Data(4, Data::Type::Int)},
														   {"uvec4", Data(4, Data::Type::UInt)},
														   {"vec4", Data(4, Data::Type::Float)},

														   {"mat4", Data(16, Data::Type::Float)},

														   {"sampler1D", Data(1, Data::Type::Int)},
														   {"sampler2D", Data(1, Data::Type::Int)},
														   {"sampler3D", Data(1, Data::Type::Int)},
														   {"samplerCube", Data(1, Data::Type::Int)},
													   })
	{
	}

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

		size_t currentOffset = 0; // Initialize offset

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
				field.offset = currentOffset;

				p_uniformBlockToFill.fields.push_back(field);

				// Update the offset based on the type size
				currentOffset += fieldData.size;
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

	void AbstractPipeline::Configuration::parseLayoutPushConstantInstruction(const std::string &p_instruction)
	{
		spk::cout << "Parsing PushConstant instruction [" << spk::to_wstring(p_instruction) << "]" << std::endl;
	}

	void AbstractPipeline::Configuration::parseLayoutBufferInstruction(const std::string &p_instruction)
	{
		spk::cout << "Parsing Buffer instruction [" << spk::to_wstring(p_instruction) << "]" << std::endl;
	}

	bool AbstractPipeline::Configuration::isUniformLayoutInstruction(const std::string &p_instruction)
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

	bool AbstractPipeline::Configuration::isPushConstantUniformInstruction(const std::string &p_instruction)
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

	void AbstractPipeline::Configuration::parseLayoutInstruction(const std::string &p_instruction)
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
		else
		{
			parseLayoutBufferInstruction(p_instruction);
		}
	}

	void AbstractPipeline::Configuration::parseStructInstruction(const std::string &p_instruction)
	{
		spk::cout << "Parsing Struct instruction [" << spk::to_wstring(p_instruction) << "]" << std::endl;
	}

	void AbstractPipeline::Configuration::parseShaderInstruction(const std::string &p_instruction)
	{
		if (p_instruction.find("layout") == 0)
		{
			parseLayoutInstruction(p_instruction);
		}
		else if (p_instruction.find("struct") == 0)
		{
			parseStructInstruction(p_instruction);
		}
	}

	void AbstractPipeline::Configuration::parseShaderCode(const std::string &p_shaderCode)
	{
		std::vector<std::string> instructions = splitShaderCodeIntoInstruction(p_shaderCode);

		for (size_t i = 0; i < instructions.size(); i++)
		{
			spk::cout << "Instruction [" << i << "] - [" << spk::to_wstring(instructions[i]) << "]" << std::endl;
		}

		for (size_t i = 0; i < instructions.size(); i++)
		{
			parseShaderInstruction(instructions[i]);
		}

		spk::cout << "Uniform blocks :" << std::endl;
		for (size_t i = 0; i < uniformBlocks.size(); i++)
		{
			spk::cout << uniformBlocks[i] << std::endl;
		}
	}

	AbstractPipeline::Configuration::Configuration(const std::string &p_vertexCode, const std::string &p_fragmentCode) : Configuration()
	{
		parseShaderCode(p_vertexCode.substr(p_vertexCode.find_first_of("\n") + 1));
		parseShaderCode(p_fragmentCode.substr(p_fragmentCode.find_first_of("\n") + 1));

		exit(1);
	}
}