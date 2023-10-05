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

	AbstractPipeline::Configuration::Configuration(const std::string &p_vertexCode, const std::string &p_fragmentCode) : Configuration()
	{
		parseShaderCode(p_vertexCode.substr(p_vertexCode.find_first_of("\n") + 1), true);
		parseShaderCode(p_fragmentCode.substr(p_fragmentCode.find_first_of("\n") + 1), false);
	}
}