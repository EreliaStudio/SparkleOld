#version 450 core

layout(location = 0) out vec4 color;

layout(set = 0, binding = 0) uniform ColorUniformBlock
{
	vec4 color;
} colorUniformBlock;

void main()
{
	color = colorUniformBlock.color;
}