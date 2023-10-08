#version 450 core
layout(location = 0) in vec2 model_space;


struct TestStructure
{
	vec2 test;
	vec4 color;
};


//Single line comment
layout(push_constant) uniform PushConstant { mat4 rotation; TestStructure struct;} totallyNotTheSameAsStruct;


void main()
{
	gl_Position = totallyNotTheSameAsStruct.rotation * vec4(model_space, 0.0f, 1.0f);
}
