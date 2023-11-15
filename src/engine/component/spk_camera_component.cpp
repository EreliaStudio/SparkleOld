#include "engine/component/spk_camera_component.hpp"
#include "engine/spk_game_object.hpp"

namespace spk
{
	void Camera::updateProjectionMatrix()
	{
		if (_type == Type::Perspective)
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

	void Camera::_updateMVP()
	{
		auto viewMatrix = spk::Matrix4x4::lookAt(
				owner()->transform()->translation(),
				owner()->transform()->translation() + owner()->transform()->forward(),
				spk::Vector3(0, 1, 0)
			);
		_MVP = (_projectionMatrix * viewMatrix * spk::Matrix4x4());
	}

	Camera::Camera(std::shared_ptr<GameObject> p_owner, Type p_type) :
		Component(p_owner, L"Camera"),
		_type(p_type),
		_translationContract(p_owner->transform()->subscribeOnTranslation([&](){
			_updateMVP();
		})),
		_rotationContract(p_owner->transform()->subscribeOnRotation([&](){
			_updateMVP();
		}))
	{
		setPerspectiveParameters(45.0f, 1.33f, 0.1f, 100.0f);
		setOrthographicParameters(spk::Vector2(20, 20), 0.1f, 100.0f);
		updateProjectionMatrix();

		if (_mainCamera == nullptr)
			setAsMainCamera();
	}

	std::shared_ptr<Camera> Camera::mainCamera()
	{
		return (std::shared_ptr<Camera>(_mainCamera, [](Camera* p_camera){}));
	}

	void Camera::setAsMainCamera()
	{
		_mainCamera = this;
	}

	void Camera::setType(Type p_type) {
		_type = p_type;
		updateProjectionMatrix();
	}

	void Camera::setPerspectiveParameters(float p_fov, float p_aspectRatio, float p_nearPlane, float p_farPlane)
	{
		_fov = p_fov;
		_aspectRatio = p_aspectRatio;
		_nearPlane = p_nearPlane;
		_farPlane = p_farPlane;
		if (_type == Type::Perspective) {
			updateProjectionMatrix();
		}
	}
	void Camera::setOrthographicParameters(const spk::Vector2& p_size, float p_nearPlane, float p_farPlane)
	{
		_size = p_size;
		_nearPlane = p_nearPlane;
		_farPlane = p_farPlane;
		if (_type == Type::Orthographic) {
			updateProjectionMatrix();
		}
	}

	spk::Matrix4x4 Camera::MVP() const
	{
		return (_MVP);
	}
}