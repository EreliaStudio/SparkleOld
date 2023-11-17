#pragma once

#include "engine/component/spk_component.hpp"
#include "engine/component/spk_transform_component.hpp"
#include "engine/spk_mesh.hpp"
#include "graphics/spk_pipeline.hpp"
#include "graphics/spk_texture.hpp"

namespace spk
{
	class MeshRenderer : public Component
	{
	protected:
    	static inline std::shared_ptr<spk::Pipeline> _renderingPipeline = nullptr;
		std::shared_ptr<spk::Pipeline::Object> _renderingObject;

		std::shared_ptr<spk::Texture> _texture;

		Transform::TranslationType::Contract _translationContract;
		Transform::ScaleType::Contract _scaleContract;
		Transform::RotationType::Contract _rotationContract;

		virtual bool _onUpdate() override;
		virtual void _onRender() override;

		std::shared_ptr<spk::Mesh> _mesh;

		void _updateMeshModelData();

	public:
		MeshRenderer();
		MeshRenderer(std::shared_ptr<spk::Mesh> p_mesh, std::shared_ptr<spk::Texture> p_texture);
	
		void setMesh(std::shared_ptr<spk::Mesh> p_mesh);
		std::shared_ptr<spk::Mesh> mesh();
		std::shared_ptr<const spk::Mesh> mesh() const;
	
		void setTexture(std::shared_ptr<spk::Texture> p_texture);
		std::shared_ptr<spk::Texture> texture();
		std::shared_ptr<const spk::Texture> texture() const;
	};
}