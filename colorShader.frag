#version 450 core

layout(location = 0) out vec4 color;

layout(binding = 0) uniform
	ColorUniform
{
	vec4 triangleColor;
} uniformData;

struct AAAAAAAAAAAAAAAAAhB
{
	vec4 position;
	vec4 color;
};

void main()
{
	color = uniformData.triangleColor;
}

// Ignored by the parsing
struct AAAAAAAAAAAAAAAAAh
{
	vec4 position;
	vec4 color;
};