#include "graphics/pipeline/spk_default_shader.hpp"

namespace spk
{
	spk::ShaderModule boxComponentVertexShaderModule = spk::ShaderModule(
"BoxComponentVertexShader",
R"(#version 450

layout(location = 0) in vec2 inPosition;

layout(push_constant) uniform PushConstant
{
	float depth;
	vec4 color;
} pushConstants;

layout(location = 0) out vec4 fragmentColor;

void main() {
    gl_Position = vec4(inPosition, pushConstants.depth, 1.0);
    fragmentColor = pushConstants.color;
})");

	spk::ShaderModule boxComponentFragmentShaderModule = spk::ShaderModule(
"BoxComponentFragmentShader", 
R"(#version 450

layout(location = 0) in vec4 fragmentColor;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = fragmentColor;
})");

    spk::ShaderModule textLabelComponentVertexShaderModule = spk::ShaderModule(
		"TextLabelComponentVertexShader",
R"(#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inUvs;

layout(push_constant) uniform PushConstant
{
	float depth;
	vec4 textColor;
	vec4 outlineColor;
} pushConstants;

layout(location = 0) out vec2 fragmentUVs;
layout(location = 1) out vec4 textColor;
layout(location = 2) out vec4 outlineColor;

void main() {
    gl_Position = vec4(inPosition, pushConstants.depth, 1.0);
	fragmentUVs = inUvs;
    textColor = pushConstants.textColor;
    outlineColor = pushConstants.outlineColor;
})"
	);

    spk::ShaderModule textLabelComponentFragmentShaderModule = spk::ShaderModule(
		"TextLabelComponentFragmentShader",
R"(#version 450

layout(location = 0) in vec2 fragmentUVs;
layout(location = 1) in vec4 textColor;
layout(location = 2) in vec4 outlineColor;

layout(binding = 0) uniform sampler2D textureID;

layout(location = 0) out vec4 outColor;

void main() {
    float r = texture(textureID, fragmentUVs).r;

    if (r == 0.0) {
        discard;
    } else if (r == 1.0) {
        outColor = textColor;
    } else {
        outColor = outlineColor;
    }
})"
	);
}