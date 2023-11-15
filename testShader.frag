#version 450 core

layout (location = 0) in vec2 fragmentUV;
layout (location = 0) out vec4 color;

void main()
{
	color = vec4(fragmentUV, 0, 1);
}