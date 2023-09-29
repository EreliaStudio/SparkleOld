#version 450 core
layout(location = 0) in vec2 model_space;
layout(location = 1) in vec4 model_color;

struct PointLight
{
    vec3 position;
    float intensity;
    vec3 color;
};

struct TotalyNotPointLight
{
    vec2 position;
    vec4 color;
	mat4 matrix;
};

layout(push_constant) uniform PushConstant
{
    //Structure to describe our point light
    PointLight light;
    TotalyNotPointLight notLight;// Totaly not used structure
} pushConstant;

layout(set = 0, binding = 0) uniform UniformBlockA
{
    //Comment to ignore
    vec2 test;
    vec4 color; //Comment to ignore
    PointLight pointLight;
} uniformA;

layout(location = 0) out vec4 fragmentColor;

void main()
{
	gl_Position = vec4(model_space, 0.0f, 1.0f);
	fragmentColor = model_color;
}