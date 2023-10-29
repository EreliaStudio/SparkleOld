#include "graphics/pipeline/spk_default_shader.hpp"

namespace spk
{
	spk::ShaderModule boxComponentVertexShaderModule = spk::ShaderModule(
"BoxComponentVertexShader",
R"(#version 450

layout(location = 0) in vec2 inPosition;

layout(push_constant) uniform PushConstant
{
	vec2 uv;
	vec4 color;
	float depth;
	mat4 rota;
} pushConstants;

out vec4 fragmentColor;

void main() {
    gl_Position = vec4(inPosition, pushConstants.depth, 1.0);
	if (pushConstants.color.r == 0 && pushConstants.color.g == 0.0 && pushConstants.color.b == 0.0 && pushConstants.color.a == 0.0)
		fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
	else
		fragmentColor = vec4(0.0, 1.0, 0.0, 1.0);
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