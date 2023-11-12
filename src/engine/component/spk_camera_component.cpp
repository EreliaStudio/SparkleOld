#include "engine/component/spk_camera_component.hpp"

namespace spk
{
	void Camera::updateProjectionMatrix()
	{
		if (_type == CameraType::Perspective)
		{
			_projectionMatrix = spk::Matrix4x4::perspective(
				spk::degreeToRadian(_fov),
				_aspectRatio,
				_nearPlane,
				_farPlane
			);
		}
		else
		{
			float halfWidth = _size.x / 2.0f;
			float halfHeight = _size.y / 2.0f;
			_projectionMatrix = spk::Matrix4x4::ortho(
				-halfWidth, halfWidth,
				-halfHeight, halfHeight,
				_nearPlane, _farPlane
			);
		}
	}

	bool Camera::_onUpdate()
	{
		return true;
	}

	void Camera::_onRender()
	{
		
	}

	Camera::Camera(std::shared_ptr<GameObject> p_owner, CameraType p_type) :
		Component(p_owner),
		_type(p_type)
	{
		setPerspectiveParameters(45.0f, 1.33f, 0.1f, 100.0f);
		setOrthographicParameters(spk::Vector2(20, 20), 0.1f, 100.0f);
		updateProjectionMatrix();
	}

	void Camera::setAsMainCamera()
	{
		_mainCamera = this;
	}

	void Camera::setType(CameraType p_type) {
		_type = p_type;
		updateProjectionMatrix();
	}

	void Camera::setPerspectiveParameters(float p_fov, float p_aspectRatio, float p_nearPlane, float p_farPlane)
	{
		_fov = p_fov;
		_aspectRatio = p_aspectRatio;
		_nearPlane = p_nearPlane;
		_farPlane = p_farPlane;
		if (_type == CameraType::Perspective) {
			updateProjectionMatrix();
		}
	}
	void Camera::setOrthographicParameters(const spk::Vector2& p_size, float p_nearPlane, float p_farPlane)
	{
		_size = p_size;
		_nearPlane = p_nearPlane;
		_farPlane = p_farPlane;
		if (_type == CameraType::Orthographic) {
			updateProjectionMatrix();
		}
	}

	spk::Matrix4x4 Camera::getMVPMatrix(const spk::Matrix4x4& modelMatrix) const
	{
		auto viewMatrix = spk::Matrix4x4::lookAt(
				owner()->transform()->position,
				owner()->transform()->position + owner()->transform()->rotation,
				spk::Vector3(0, 1, 0)
			);
		return (_projectionMatrix * viewMatrix * modelMatrix);
	}
}