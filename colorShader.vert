#version 450 core
layout(location = 0) in vec2 model_space;

layout(push_constant) uniform PushConstant
{
    vec4 color;
} totalyNotTheSameAsStruct;

layout(location = 0) out vec4 fragmentColor;

void main()
{
	gl_Position = vec4(model_space, 0.0f, 1.0f);
	fragmentColor = totalyNotTheSameAsStruct.color;
}