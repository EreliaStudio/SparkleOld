#include "graphics/pipeline/spk_default_shader.hpp"

namespace spk
{
	spk::ShaderModule boxComponentVertexShaderModule = spk::ShaderModule(
"BoxComponentVertexShader",
R"(#version 450

layout(location = 0) in vec2 inPosition;

struct MyStruct
{
	vec2 A;
	vec4 B;
	vec2 C;
	vec4 D;
	vec2 E;
	vec4 F;
	vec2 G;
	vec4 H;
	float W;
	float X;
	float Y;
	float Z;
	float ZA;
};

layout(push_constant) uniform PushConstant
{
	float A;
	MyStruct B;
	float C;
	mat4 D;
	float E;
	vec4 F;
	float G;
	vec2 H;
	mat4 I;
	vec4 L;
	float P;
	MyStruct S;
	float Z;
} pushConstants;

out vec4 fragmentColor;

void main() {
    gl_Position = vec4(inPosition, 0.0, 1.0);
	fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
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