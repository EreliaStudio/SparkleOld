#pragma once

#include <memory>
#include "engine/component/spk_component.hpp"
#include "math/spk_vector2.hpp"
#include "math/spk_matrix4x4.hpp"
#include "engine/component/spk_transform_component.hpp"
#include "graphics/spk_pipeline.hpp"

namespace spk
{
	class Camera : public Component
	{
	public:
		enum class Type {
			Perspective,
			Orthographic
		};

	private:
		static inline Camera* _mainCamera = nullptr;
		Type _type;
		spk::Matrix4x4 _projectionMatrix;
		spk::Matrix4x4 _MVP;
		bool _positionEdited = false;
		bool _MVPEdited = false;
		std::shared_ptr<Transform::TranslationType::Contract> _translationContract;
		std::shared_ptr<Transform::RotationType::Contract> _rotationContract;

		// --- Perspective camera attributes
		float _fov;
		float _aspectRatio;
		float _nearPlane;
		float _farPlane;

		// --- Orthographic camera attributes
		spk::Vector2 _size;

		void _updateProjectionMatrix();

		void _updateMVP();

	protected:
		virtual bool _onUpdate() override;
		virtual void _onRender() override;

	public:
		Camera(Type p_type = Type::Perspective);
		~Camera();

		static std::shared_ptr<Camera> mainCamera();

		void setAsMainCamera();
		void setType(Type p_type);
		void setPerspectiveParameters(float p_fov, float p_aspectRatio, float p_nearPlane, float p_farPlane);
		void setOrthographicParameters(const spk::Vector2& p_size, float p_nearPlane, float p_farPlane);

		bool MVPEdited() const;
		void pushMVP(AbstractPipeline::UniformBlock::Field & p_uniformField);

		const spk::Matrix4x4& MVP() const;
	};
}