#version 450 core

in vec2 uvs;

layout(location = 0) out vec4 color;

layout(binding = 0) uniform sampler2D textureID;

void main()
{
	color = texture(textureID, uvs);
}