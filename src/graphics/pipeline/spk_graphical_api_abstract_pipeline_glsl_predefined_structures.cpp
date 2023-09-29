#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"
#include <regex>

namespace spk::GraphicalAPI
{
	std::string predefinedStructures = R"(
// Predefined structure A
struct PredefinedStructA
{
	vec3 position;
	vec3 scale;
	vec3 rotation;
};

// Predefined structure B
struct PredefinedStructB
{
	vec3 origin;
	mat4 mat;
};
)";
}