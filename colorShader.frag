#version 450 core

layout(location = 0) out vec4 color;

layout(binding = 0) uniform ColorUniform
{
	vec4 triangleColor;
} uniformData;

void main()
{
	color = uniformData.triangleColor;
}