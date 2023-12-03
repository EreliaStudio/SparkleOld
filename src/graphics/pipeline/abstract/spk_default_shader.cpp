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

    spk::ShaderModule meshRendererComponentVertexShaderModule = spk::ShaderModule("MeshRendererComponentVertexShaderModule", R"(#version 450 core

layout (location = 0) in vec3 model_space;
layout (location = 1) in vec2 model_uv;
layout (location = 2) in vec3 model_normal;

layout (location = 0) out vec2 fragmentUV;
layout (location = 1) out vec3 fragmentNormal;

layout(push_constant) uniform PushConstants
{
	vec3 translation;
	vec3 scale;
	vec3 rotation;
} pushConstants;

layout(binding = 0) uniform CameraInformation
{
	mat4 MVP;
} cameraInformation;

mat4 createRotationMatrix(vec3 angles)
{
    float c1 = cos(angles.x);
    float s1 = sin(angles.x);
    float c2 = cos(angles.y);
    float s2 = sin(angles.y);
    float c3 = cos(angles.z);
    float s3 = sin(angles.z);

    mat4 rotationX = mat4(1.0f, 0.0f, 0.0f, 0.0f,
                          0.0f,   c1,   s1, 0.0f,
                          0.0f,  -s1,   c1, 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);

    mat4 rotationY = mat4(  c2, 0.0f,  -s2, 0.0f,
                          0.0f, 1.0f, 0.0f, 0.0f,
                            s2, 0.0f,   c2, 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);

    mat4 rotationZ = mat4(  c3,   s3, 0.0f, 0.0f,
                           -s3,   c3, 0.0f, 0.0f,
                          0.0f, 0.0f, 1.0f, 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);

    return rotationZ * rotationY * rotationX;
}

void main()
{
	mat4 rotationMatrix = createRotationMatrix(pushConstants.rotation);
    vec3 rotatedPosition = (rotationMatrix * vec4(model_space * pushConstants.scale, 1.0)).xyz;
	vec3 scaledPosition = pushConstants.translation + rotatedPosition;
	gl_Position = cameraInformation.MVP * vec4(scaledPosition, 1.0f);
	fragmentUV = model_uv;

    fragmentNormal = model_normal;
})");

    spk::ShaderModule meshRendererComponentFragmentShaderModule = spk::ShaderModule("MeshRendererComponentFragmentShaderModule", R"(#version 450 core

layout (location = 0) in vec2 fragmentUV;
layout (location = 1) in vec3 fragmentNormal;
layout (location = 0) out vec4 color;

layout (binding = 1) uniform sampler2D textureID;

layout (binding = 0) uniform CameraInformation
{
	mat4 MVP;
} cameraInformation;

layout (binding = 2) uniform LightingInformation
{
	vec3 directionalLight;
} lightingInformation;

float computeDiffuse()
{
    float result = dot(fragmentNormal, lightingInformation.directionalLight);

    return (result);
}

vec4 generateColor(sampler2D textureID, vec2 uv)
{
    float diffuse = computeDiffuse();
    float ambiant = 0.25f;
    
    float lightIntensity = min(diffuse + ambiant, 1.0f);

    vec4 texColor = texture(textureID, uv);
    vec4 result = texColor * lightIntensity;
    result.a = 1.0f;
    
    return (result);
}

void main()
{
    color = generateColor(textureID, fragmentUV);
})");
}