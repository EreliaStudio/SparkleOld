#version 450 core
layout(location = 0) in vec2 model_space;

/* This is a block comment */

layout(push_constant) uniform PushConstant
{
	mat4 rotation;
} totallyNotTheSameAsStruct;

void functionTest()
{
	
}

struct Test{
	vec2 position;
	vec4 color;
};

// This is an inline comment
void main()
{
	gl_Position = totallyNotTheSameAsStruct.rotation * vec4(model_space, 0.0f, 1.0f);
}