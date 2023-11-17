#include "engine/component/spk_mesh_renderer_component.hpp"
#include "graphics/pipeline/spk_default_shader.hpp"
#include "engine/spk_game_object.hpp"

namespace spk
{
	MeshRenderer::MeshRenderer() :
		Component(L"MeshRenderer"),
		_mesh(nullptr),
		_texture(nullptr),
		_translationContract(owner()->transform()->subscribeOnTranslation([&](){
			_renderingObject->pushConstants(L"translation") = owner()->transform()->translation();
		})),
		_scaleContract(owner()->transform()->subscribeOnScaling([&](){
			_renderingObject->pushConstants(L"scale") = owner()->transform()->scale();
		})),
		_rotationContract(owner()->transform()->subscribeOnRotation([&](){
			_renderingObject->pushConstants(L"rotation") = owner()->transform()->rotation();
		}))
	{
		if (_renderingPipeline == nullptr)
		{
			_renderingPipeline = std::make_shared<spk::Pipeline>(meshRendererComponentVertexShaderModule, meshRendererComponentFragmentShaderModule);
		}
		_renderingObject = _renderingPipeline->createObject();
	}

	MeshRenderer::MeshRenderer(std::shared_ptr<spk::Mesh> p_mesh, std::shared_ptr<spk::Texture> p_texture) :
		MeshRenderer()
	{
		setMesh(p_mesh);
		setTexture(p_texture);
	}

	bool MeshRenderer::_onUpdate()
	{
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

		// if (_texture == nullptr)
		// 	spk::throwException(L"Can't render a MeshRenderer without texture");
		
		if (_mesh->needUpdate() == true)
		{
			_updateMeshModelData();
		}
		
		// _texture->bind(0);
		// _renderingPipeline->uniform(L"textureID") = 0;
		_renderingObject->render();
		// _texture->unbind();
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