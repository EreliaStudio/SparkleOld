#version 330 core
layout(location = 0) in vec2 model_space;

void main()
{
	gl_Position = vec4(model_space, 0.0f, 1.0f);
}