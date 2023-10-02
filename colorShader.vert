#version 450 core
layout(location = 0) in vec2 model_space;

layout(push_constant) uniform PushConstant
{
	mat4 rotation;
} totalyNotTheSameAsStruct;

void main()
{
	gl_Position = totalyNotTheSameAsStruct.rotation * vec4(model_space, 0.0f, 1.0f);
}