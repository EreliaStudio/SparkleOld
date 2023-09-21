#version 330 core
layout(location = 0) in vec2 model_space;
in vec4 model_color;

uniform vec2 delta;
out vec4 fragmentColor;

void main()
{
	gl_Position = vec4(model_space + delta, 0.0f, 1.0f);
	fragmentColor = model_color;
}