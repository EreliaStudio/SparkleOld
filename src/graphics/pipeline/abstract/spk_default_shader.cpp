#include "graphics/pipeline/spk_default_shader.hpp"

namespace spk
{
	spk::ShaderModule boxComponentVertexShaderModule = spk::ShaderModule(
"BoxComponentVertexShader",
R"(#version 450

layout(location = 0) in vec2 inPosition;

layout(push_constant) uniform PushConstant
{
	vec4 color;
	float depth;
} pushConstants;

out vec4 fragmentColor;

void main() {
    gl_Position = vec4(inPosition, pushConstants.depth, 1.0);
	fragmentColor = pushConstants.color;
})");

	spk::ShaderModule boxComponentFragmentShaderModule = spk::ShaderModule(
"BoxComponentFragmentShader", 
R"(#version 450

in vec4 fragmentColor;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = fragmentColor;
})");
}