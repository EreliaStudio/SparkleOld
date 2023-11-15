#version 450 core

layout (location = 0) in vec3 model_space;
layout (location = 1) in vec2 model_uv;
layout (location = 2) in vec3 model_normal;

layout (location = 0) out vec2 fragmentUV;

layout(binding = 0) uniform CameraInformation
{
	mat4 MVP;
} cameraInformation;

void main()
{
	gl_Position = cameraInformation.MVP * vec4(model_space, 1.0f);
	fragmentUV = model_uv;
}
