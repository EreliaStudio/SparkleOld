#version 450 core

in vec2 uvs;

layout(binding = 1) uniform sampler2D textureID;

layout(location = 0) out vec4 color;

void main()
{
	color = texture(textureID, uvs);
}