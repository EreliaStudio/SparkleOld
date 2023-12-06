#include "engine/component/spk_mesh_renderer_component.hpp"
#include "engine/spk_game_object.hpp"

namespace spk
{
	
    spk::ShaderModule MeshRenderer::VertexShaderModule = spk::ShaderModule("MeshRendererComponentVertexShaderModule", R"(#version 450 core

layout (location = 0) in vec3 model_space;
layout (location = 1) in vec2 model_uv;
layout (location = 2) in vec3 model_normal;

layout (location = 0) out vec3 fragmentPosition;
layout (location = 1) out vec2 fragmentUV;
layout (location = 2) out vec3 fragmentNormal;

layout(push_constant) uniform PushConstants
{
	vec3 translation;
	vec3 scale;
	vec3 rotation;
    Material material;
} pushConstants;

layout(binding = 0) uniform CameraInformation
{
    vec3 position;
	mat4 MVP;
} cameraInformation;

mat4 createRotationMatrix(vec3 angles)
{
    float c1 = cos(angles.x);
    float s1 = sin(angles.x);
    float c2 = cos(angles.y);
    float s2 = sin(angles.y);
    float c3 = cos(angles.z);
    float s3 = sin(angles.z);

    mat4 rotationX = mat4(1.0f, 0.0f, 0.0f, 0.0f,
                          0.0f,   c1,   s1, 0.0f,
                          0.0f,  -s1,   c1, 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);

    mat4 rotationY = mat4(  c2, 0.0f,  -s2, 0.0f,
                          0.0f, 1.0f, 0.0f, 0.0f,
                            s2, 0.0f,   c2, 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);

    mat4 rotationZ = mat4(  c3,   s3, 0.0f, 0.0f,
                           -s3,   c3, 0.0f, 0.0f,
                          0.0f, 0.0f, 1.0f, 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);

    return rotationZ * rotationY * rotationX;
}

void main()
{
	mat4 rotationMatrix = createRotationMatrix(pushConstants.rotation);
    vec3 rotatedPosition = (rotationMatrix * vec4(model_space * pushConstants.scale, 1.0)).xyz;
	vec3 scaledPosition = pushConstants.translation + rotatedPosition;
	gl_Position = cameraInformation.MVP * vec4(scaledPosition, 1.0f);

	fragmentPosition = scaledPosition;
	fragmentUV = model_uv;
    fragmentNormal = model_normal;
})");

    spk::ShaderModule MeshRenderer::FragmentShaderModule = spk::ShaderModule("MeshRendererComponentFragmentShaderModule", R"(#version 450 core


layout (location = 0) in vec3 fragmentPosition;
layout (location = 1) in vec2 fragmentUV;
layout (location = 2) in vec3 fragmentNormal;

layout (location = 0) out vec4 outputColor;

layout (binding = 1) uniform sampler2D textureID;

layout(push_constant) uniform PushConstants
{
	vec3 translation;
	vec3 scale;
	vec3 rotation;
    Material material;
} pushConstants;

layout (binding = 0) uniform CameraInformation
{
    vec3 position;
	mat4 MVP;
} cameraInformation;

layout (binding = 2) uniform LightingInformation
{
	DirectionalLight directionalLight;
} lightingInformation;

vec4 computeDiffuseRatio(vec3 p_normal, vec3 p_lightDirection)
{
	float dotValue = dot(normalize(p_normal), normalize(p_lightDirection));
	float ratio = 1.0f;
	if (dotValue > 0)
		ratio = (1 - (dotValue + lightingInformation.directionalLight.intensity));

	return (vec4(ratio, ratio, ratio, 1));
}

vec4 computeAmbiantRatio(float p_intensity)
{
	return (vec4(p_intensity, p_intensity, p_intensity, 1));
}

vec4 computeSpecularRatio(float p_specularPower, float p_specularIntensity, vec3 p_eyePosition, vec3 p_worldPosition, vec3 p_normal, vec3 p_lightDirection)
{
	float ratio = 0.0f;

	vec3 eyeToWorldNormal = normalize(p_eyePosition - p_worldPosition);
    vec3 lightReflection = normalize(reflect(p_lightDirection, p_normal));

	float specularRatio = dot(eyeToWorldNormal, lightReflection);
	if (specularRatio > 0)
		ratio = pow(specularRatio, p_specularPower) * p_specularIntensity;
	
	return (vec4(ratio, ratio, ratio, 1));
}

void main()
{
    vec4 textureColor = texture(textureID, fragmentUV);

	vec4 diffuseColor  = lightingInformation.directionalLight.color * computeDiffuseRatio(fragmentNormal, lightingInformation.directionalLight.direction);
	vec4 ambiantColor  = lightingInformation.directionalLight.color * computeAmbiantRatio(lightingInformation.directionalLight.intensity);
	vec4 specularColor = lightingInformation.directionalLight.color * computeSpecularRatio(pushConstants.material.specularPower, pushConstants.material.specularIntensity, cameraInformation.position, fragmentPosition, fragmentNormal, lightingInformation.directionalLight.direction);
	
	vec4 fuzedColor = min(diffuseColor + ambiantColor, vec4(1, 1, 1, 1)) + specularColor;
	outputColor = textureColor * fuzedColor;
})");

	void MeshRenderer::initializeMeshRendererShader()
	{
		_renderingPipeline = std::make_shared<spk::Pipeline>(VertexShaderModule, FragmentShaderModule);

		_textureIDUniform = dynamic_pointer_cast<spk::Pipeline::SamplerUniform>(_renderingPipeline->uniform(L"textureID"));
		_textureIDUniform->set(0);
	}
		
	MeshRenderer::MeshRenderer() :
		Component(L"MeshRenderer"),
		_mesh(nullptr),
		_texture(nullptr),
		_translationContract(owner()->transform()->subscribeOnTranslation([&](){
			_updateTransform = true;	
		})),
		_scaleContract(owner()->transform()->subscribeOnScaling([&](){
			_updateTransform = true;	
		})),
		_rotationContract(owner()->transform()->subscribeOnRotation([&](){
			_updateTransform = true;	
		}))
	{
		
	}

	MeshRenderer::MeshRenderer(std::shared_ptr<spk::Mesh> p_mesh, std::shared_ptr<spk::Texture> p_texture) :
		MeshRenderer()
	{
		setMesh(p_mesh);
		setTexture(p_texture);
	}

	void MeshRenderer::_updateMeshModelData()
	{
		_renderingObject->storage().vertices() << _mesh->vertices() << std::endl;
		_renderingObject->storage().indexes() << _mesh->indexes() << std::endl;

		_mesh->resetNeedUpdateFlag();
	}

	void MeshRenderer::_updateMaterial()
	{
		if (_lastMaterial != *_material)
		{
			_renderingObject->pushConstants(L"material") = *_material;
			_lastMaterial = *_material;
		}
	}

	bool MeshRenderer::_onUpdate()
	{
		if (_renderingObject == nullptr)
			return (false);

		if (_updateTransform == true)
		{
			_renderingObject->pushConstants(L"translation") = owner()->transform()->translation();
			_renderingObject->pushConstants(L"scale") = owner()->transform()->scale();
			_renderingObject->pushConstants(L"rotation") = owner()->transform()->rotation();
			_updateTransform = false;	
		}

		if (_mesh != nullptr && _mesh->needUpdate() == true)
		{
			_updateMeshModelData();
		}

		if (_material != nullptr)
		{
			_updateMaterial();
		}

		return (false);
	}

	void MeshRenderer::_onRender()
	{
		if (_mesh == nullptr)
			return ;

		if (_texture == nullptr)
			spk::throwException(L"Can't render a MeshRenderer without texture");

		if (_renderingObject == nullptr)
		{
			_renderingObject = _renderingPipeline->createObject();
		}

		_texture->bind(0);
		_textureIDUniform->bind();

		_renderingObject->render();

		_texture->unbind();
	} 
	
	void MeshRenderer::setMesh(std::shared_ptr<spk::Mesh> p_mesh)
	{
		_mesh = p_mesh;
		_mesh->setNeedUpdateFlag(true);
	}
	
	std::shared_ptr<spk::Mesh> MeshRenderer::mesh()
	{
		return (_mesh);
	}
	
	std::shared_ptr<const spk::Mesh> MeshRenderer::mesh() const
	{
		return (_mesh);
	}

	void MeshRenderer::setMaterial(std::shared_ptr<spk::Material> p_material)
	{
		_material = p_material;
		if (_material != nullptr)
			_lastMaterial.specularIntensity = _material->specularIntensity + 1.0f;
	}

	std::shared_ptr<spk::Material> MeshRenderer::material()
	{
		return (_material);
	}

	std::shared_ptr<const spk::Material> MeshRenderer::material() const
	{
		return (_material);
	}
	
	void MeshRenderer::setTexture(std::shared_ptr<spk::Texture> p_texture)
	{
		_texture = p_texture;
	}

	std::shared_ptr<spk::Texture> MeshRenderer::texture()
	{
		return (_texture);
	}

	std::shared_ptr<const spk::Texture> MeshRenderer::texture() const
	{
		return (_texture);
	}
}