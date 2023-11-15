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

    spk::ShaderModule meshRendererComponentVertexShaderModule = spk::ShaderModule("MeshRendererComponentVertexShaderModule", R"(#version 450

layout(push_constant) uniform PushConstant {
    vec3 translation;
    vec3 scale;
    vec3 rotation;
} pushConstants;

layout(location = 0) in vec3 modelPosition;
layout(location = 1) in vec2 modelUV;
layout(location = 2) in vec3 modelNormal;

layout(binding = 0) uniform RenderStat {
    mat4 MVP;
};

out vec2 fragUV;
out vec3 fragNormal;

mat4 rotateX(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0,   c,   s, 0.0,
        0.0,  -s,   c, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 rotateY(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return mat4(
        c, 0.0,   -s, 0.0,
        0.0, 1.0, 0.0, 0.0,
        s, 0.0,    c, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 rotateZ(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return mat4(
          c,   s, 0.0, 0.0,
         -s,   c, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 rotateXYZ(vec3 rotationAngles)
{
    //Order important
    return (
            rotateZ(rotationAngles.z) *
            rotateY(rotationAngles.y) *
            rotateX(rotationAngles.x)
        );
}

void main() {
    vec4 transformedPosition = rotateXYZ(pushConstants.rotation) * vec4(modelPosition * pushConstants.scale, 1.0);
    transformedPosition = transformedPosition + vec4(pushConstants.translation, 0.0);
    
    gl_Position = MVP * transformedPosition;
    fragUV = modelUV;
    fragNormal = modelNormal;
}
    )");

    spk::ShaderModule meshRendererComponentFragmentShaderModule = spk::ShaderModule("MeshRendererComponentFragmentShaderModule", R"(#version 450

layout(binding = 1) uniform GlobalLighting {
    vec3 directionalLight;
};

layout(binding = 2) uniform sampler2D textureID;

in vec2 fragUV;
in vec3 fragNormal;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(directionalLight);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * texture(textureID, fragUV).rgb;

    // Appliquer un éclairage simple
    outColor = vec4(diffuse, 1.0);
}
    )");
}