#pragma once

#include <memory>
#include "engine/component/spk_component.hpp"
#include "math/spk_vector2.hpp"
#include "math/spk_matrix4x4.hpp"

namespace spk
{
	class Camera : public Component
	{
	public:
		enum class CameraType {
			Perspective,
			Orthographic
		};

	private:
		static inline Camera* _mainCamera = nullptr;
		CameraType _type;
		spk::Matrix4x4 _projectionMatrix;

		// --- Perspective camera attributes
		float _fov;
		float _aspectRatio;
		float _nearPlane;
		float _farPlane;

		// --- Orthographic camera attributes
		spk::Vector2 _size;

		void updateProjectionMatrix();

	protected:
		virtual bool _onUpdate() override;
		virtual void _onRender() override;

	public:
		Camera(std::shared_ptr<GameObject> p_owner, CameraType p_type = CameraType::Perspective);

		static std::shared_ptr<Camera> mainCamera();

		void setAsMainCamera();
		void setType(CameraType p_type);
		void setPerspectiveParameters(float p_fov, float p_aspectRatio, float p_nearPlane, float p_farPlane);
		void setOrthographicParameters(const spk::Vector2& p_size, float p_nearPlane, float p_farPlane);
		spk::Matrix4x4 MVP() const;
	};
}