#version 450 core

in vec2 uvs;

layout(binding = 1) uniform sampler2D textureID;

layout(location = 0) out vec4 color;

void main()
{
	color = texture(textureID, uvs);
	if (color.r == 0)
		discard;
	if (color.r == 1)
		color = vec4(1.0, 1.0, 1.0, 1.0f);
	else
		color = vec4(1.0, 0, 0, 1);
}