#version 450 core
layout(location = 0) in vec2 model_space;

//Single line comment
layout(push_constant) uniform PushConstant
{
	mat4 rotation;
} totallyNotTheSameAsStruct;


layout(set = 1, binding = 1) uniform mat4 MVP;
layout(binding = 2) uniform mat4 MVP2;

struct TestStructure
{
	vec2 test;
	vec4 color;
};

/*
Multiline comment
*/

void main()
{
	gl_Position = totallyNotTheSameAsStruct.rotation * vec4(model_space, 0.0f, 1.0f);
}
