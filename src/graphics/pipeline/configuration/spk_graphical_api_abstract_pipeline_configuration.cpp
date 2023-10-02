#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"
#include <regex>

namespace spk::GraphicalAPI
{
	AbstractPipeline::Configuration::Configuration() :
		dataTypes({
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

	std::vector<std::string> splitShaderCodeIntoInstruction(const std::string &p_shaderCode)
	{
		std::vector<std::string> result;
		std::string currentInstruction;
		int braceLevel = 0;

		for (size_t i = 0; i < p_shaderCode.size(); ++i)
		{
			char c = p_shaderCode[i];

			if (c == '{')
				++braceLevel;
			else if (c == '}')
				--braceLevel;

			currentInstruction += c;

			if (c == ';' && braceLevel == 0)
			{
				result.push_back(currentInstruction);
				currentInstruction.clear();
			}
		}
		return (result);
	}

	void AbstractPipeline::Configuration::parseLayoutUniformInstruction(const std::string &p_instruction)
	{

	}
	
	void AbstractPipeline::Configuration::parseLayoutBufferInstruction(const std::string &p_instruction)
	{

	}
	
	bool AbstractPipeline::Configuration::isUniformLayoutInstruction(const std::string &p_instruction)
	{
		std::vector<std::string> tab = 

		return (false);
	}

	void AbstractPipeline::Configuration::parseLayoutInstruction(const std::string &p_instruction)
	{
		if (isUniformLayoutInstruction(p_instruction) == true)
		{
			parseLayoutUniformInstruction(p_instruction);
		}
		else
		{
			parseLayoutBufferInstruction(p_instruction);
		}
	}
	
	void AbstractPipeline::Configuration::parseStructInstruction(const std::string &p_instruction)
	{
		spk::cout << "Parsing Struct : " << spk::to_wstring(p_instruction) << std::endl;
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
		else
		{
			spk::throwException(L"Unexpected instruction");
		}
	}
	
	void AbstractPipeline::Configuration::parseShaderCode(const std::string &p_shaderCode)
	{
		std::vector<std::string> instructions = splitShaderCodeIntoInstruction(p_shaderCode);

		for (size_t i = 0; i < instructions.size(); i++)
		{
			parseShaderInstruction(instructions[i]);
		}
	}

	AbstractPipeline::Configuration::Configuration(const std::string& p_vertexCode, const std::string& p_fragmentCode) :
		Configuration()
	{
		parseShaderCode(p_vertexCode.substr(p_vertexCode.find_first_of("\n") + 1));
		parseShaderCode(p_fragmentCode.substr(p_fragmentCode.find_first_of("\n") + 1));
	}
}