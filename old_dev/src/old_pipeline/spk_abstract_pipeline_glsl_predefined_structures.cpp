#include "graphics/pipeline/spk_abstract_pipeline.hpp"
#include <regex>

namespace spk
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