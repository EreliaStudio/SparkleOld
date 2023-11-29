#include "engine/component/spk_mesh_renderer_component.hpp"
#include "graphics/pipeline/spk_default_shader.hpp"
#include "engine/spk_game_object.hpp"

namespace spk
{
	void MeshRenderer::initializeMeshRendererShader()
	{
		_renderingPipeline = std::make_shared<spk::Pipeline>(meshRendererComponentVertexShaderModule, meshRendererComponentFragmentShaderModule);

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

		return (false);
	}

	void MeshRenderer::_updateMeshModelData()
	{
		_renderingObject->storage().vertices() << _mesh->vertices() << std::endl;
		_renderingObject->storage().indexes() << _mesh->indexes() << std::endl;

		_mesh->resetNeedUpdateFlag();
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

		if (_mesh->needUpdate() == true)
		{
			_updateMeshModelData();
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