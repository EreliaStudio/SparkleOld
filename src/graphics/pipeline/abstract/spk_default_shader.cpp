#include "graphics/pipeline/spk_default_shader.hpp"

namespace spk
{
	spk::ShaderModule boxComponentVertexShaderModule = spk::ShaderModule(
"BoxComponentVertexShader",
R"(#version 450

layout(location = 0) in vec2 inPosition;

void main() {
    gl_Position = vec4(inPosition, 0.0, 1.0);
})");

	spk::ShaderModule boxComponentFragmentShaderModule = spk::ShaderModule(
"BoxComponentFragmentShader", 
R"(#version 450

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(1.0, 1.0, 1.0, 1.0);
})");
}