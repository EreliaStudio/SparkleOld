#version 450 core

layout(location = 0) out vec4 color;

layout(binding = 0) uniform ColorUniform
{
	vec4 triangleColor;
} uniformData;

layout(set = 0, binding = 1) uniform sampler2D textureID;
layout(binding = 2) uniform sampler2D normalID;

void main()
{
	color = uniformData.triangleColor;
}