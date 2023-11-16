#version 450 core

layout (location = 0) in vec3 model_space;
layout (location = 1) in vec2 model_uv;
layout (location = 2) in vec3 model_normal;

layout (location = 0) out vec2 fragmentUV;

layout(push_constant) uniform PushConstants
{
	vec3 translation;
} pushConstants;

layout(binding = 0) uniform CameraInformation
{
	float depth;
	mat4 MVP;
} cameraInformation;

void main()
{
	gl_Position = cameraInformation.MVP * vec4(pushConstants.translation + model_space, 1.0f);
	//if (cameraInformation.depth == 0)
	//	gl_Position = vec4(0, 0, 0, 0);
	fragmentUV = model_uv;
}
