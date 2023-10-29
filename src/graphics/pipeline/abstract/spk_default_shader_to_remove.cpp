#include "graphics/pipeline/spk_default_shader.hpp"

namespace spk
{
	spk::ShaderModule boxComponentVertexShaderModule1 = spk::ShaderModule(
"BoxComponentVertexShader",
R"(#version 450

layout(location = 0) in vec2 inPosition;

layout(push_constant) uniform PushConstant
{
	float depth;
	mat4 rota;
	vec4 color;
	vec2 uv;
} pushConstants;

out vec4 fragmentColor;

void main() {
    gl_Position = vec4(inPosition, pushConstants.depth, 1.0);
	if (pushConstants.color.r == 0 && pushConstants.color.g == 0.0 && pushConstants.color.b == 0.0 && pushConstants.color.a == 0.0)
		fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
	else
		fragmentColor = vec4(0.0, 1.0, 0.0, 1.0);
})");

	spk::ShaderModule boxComponentFragmentShaderModule1 = spk::ShaderModule(
"BoxComponentFragmentShader", 
R"(#version 450

in vec4 fragmentColor;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = fragmentColor;
})");



	spk::ShaderModule boxComponentVertexShaderModule2 = spk::ShaderModule(
"BoxComponentVertexShader",
R"(#version 450

layout(location = 0) in vec2 inPosition;

layout(push_constant) uniform PushConstant
{
	mat4 rota;
	float depth;
	vec4 color;
	vec2 uv;
} pushConstants;

out vec4 fragmentColor;

void main() {
    gl_Position = vec4(inPosition, pushConstants.depth, 1.0);
	if (pushConstants.color.r == 0 && pushConstants.color.g == 0.0 && pushConstants.color.b == 0.0 && pushConstants.color.a == 0.0)
		fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
	else
		fragmentColor = vec4(0.0, 1.0, 0.0, 1.0);
})");

	spk::ShaderModule boxComponentFragmentShaderModule2 = spk::ShaderModule(
"BoxComponentFragmentShader", 
R"(#version 450

in vec4 fragmentColor;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = fragmentColor;
})");



	spk::ShaderModule boxComponentVertexShaderModule3 = spk::ShaderModule(
"BoxComponentVertexShader",
R"(#version 450

layout(location = 0) in vec2 inPosition;

layout(push_constant) uniform PushConstant
{
	mat4 rota;
	vec4 color;
	float depth;
	vec2 uv;
};

out vec4 fragmentColor;

void main() {
    gl_Position = vec4(inPosition, depth, 1.0);
	if (color.r == 0 && color.g == 0.0 && color.b == 0.0 && color.a == 0.0)
		fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
	else
		fragmentColor = vec4(0.0, 1.0, 0.0, 1.0);
})");

	spk::ShaderModule boxComponentFragmentShaderModule3 = spk::ShaderModule(
"BoxComponentFragmentShader", 
R"(#version 450

in vec4 fragmentColor;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = fragmentColor;
})");



	spk::ShaderModule boxComponentVertexShaderModule4 = spk::ShaderModule(
"BoxComponentVertexShader",
R"(#version 450

layout(location = 0) in vec2 inPosition;

layout(push_constant) uniform PushConstant
{
	mat4 rota;
	vec4 color;
	vec2 uv;
	float depth;
} pushConstants;

out vec4 fragmentColor;

void main() {
    gl_Position = vec4(inPosition, pushConstants.depth, 1.0);
	if (pushConstants.color.r == 0 && pushConstants.color.g == 0.0 && pushConstants.color.b == 0.0 && pushConstants.color.a == 0.0)
		fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
	else
		fragmentColor = vec4(0.0, 1.0, 0.0, 1.0);
})");

	spk::ShaderModule boxComponentFragmentShaderModule4 = spk::ShaderModule(
"BoxComponentFragmentShader", 
R"(#version 450

in vec4 fragmentColor;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = fragmentColor;
})");
}