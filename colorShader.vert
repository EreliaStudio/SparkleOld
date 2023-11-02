#version 450 core
layout(location = 0) in vec2 model_space;
layout(location = 1) in vec2 model_uvs;

out vec2 uvs;

void main()
{
	gl_Position = vec4(model_space, 0.0f, 1.0f);
	uvs = model_uvs;
}
