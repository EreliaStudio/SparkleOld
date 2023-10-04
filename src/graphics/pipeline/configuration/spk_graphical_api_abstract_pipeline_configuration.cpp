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

	void AbstractPipeline::Configuration::parseStructInstruction(const std::string &p_instruction)
	{
		std::string input = "struct Test{vec2 position;vec4 color;}";
		std::regex re(R"(struct\s+(\w+)\s*\{([^\}]+)\})");
		std::smatch match;

		if (std::regex_search(input, match, re)) {
			std::string structName = match[1].str();
			std::string innerData = match[2].str();

			size_t structureSize = 0;

			std::regex re2(R"((\w+)\s+(\w+)\s*;)");
			std::string::const_iterator searchStart(innerData.cbegin());
			while (std::regex_search(searchStart, innerData.cend(), match, re2)) {
				std::string type = match[1];
				std::string varName = match[2];

				if (dataTypes.contains(type) == false)
				{
					spk::throwException(L"Structure [" + spk::to_wstring(type) + L"] not recognized");
				}

				auto& data = dataTypes.at(type);

				structureSize += data.size * data.format;

				searchStart = match.suffix().first;
			}

			dataTypes[structName] = Data(structureSize);
		}
	}

	AbstractPipeline::Configuration::Configuration(const std::string &p_vertexCode, const std::string &p_fragmentCode) : Configuration()
	{
		parseShaderCode(p_vertexCode.substr(p_vertexCode.find_first_of("\n") + 1));
		parseShaderCode(p_fragmentCode.substr(p_fragmentCode.find_first_of("\n") + 1));
	}
}