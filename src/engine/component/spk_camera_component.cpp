#include "engine/component/spk_camera_component.hpp"
#include "engine/spk_game_object.hpp"

namespace spk
{
	void Camera::_updateProjectionMatrix()
	{
		if (_type == Type::Perspective)
		{
			_projectionMatrix = spk::Matrix4x4::perspective(
				_fov,
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
		_updateMVP();
	}

	bool Camera::_onUpdate()
	{
		if (_positionEdited == true)
		{
			_updateMVP();
			_positionEdited = false;	
		}
		
		if (spk::Camera::mainCamera() == this && spk::Camera::mainCamera()->_needUpdate == true)
		{
			if (_cameraUniformBlock == nullptr)
			{
				_cameraUniformBlock = dynamic_pointer_cast<spk::Pipeline::UniformBlock>(spk::Pipeline::uniform(spk::Pipeline::Uniform::Key(0, 0)));
			}

			_cameraUniformBlock->field(L"MVP") = _MVP;
			spk::Camera::mainCamera()->_needUpdate = false;
		}

		return (false);
	}

	void Camera::_onRender()
	{
		
	}

	void Camera::_updateMVP()
	{
		spk::Matrix4x4 viewMatrix = spk::Matrix4x4::lookAt(
			owner()->transform()->translation(),
			owner()->transform()->translation() + owner()->transform()->forward(),
			spk::Vector3(0, 1, 0)
		);

		_MVP = (_projectionMatrix * viewMatrix * spk::Matrix4x4());
		_needUpdate = true;
	}

	Camera::Camera(Type p_type) :
		Component(L"Camera"),
		_type(p_type),
		_translationContract(owner()->transform()->subscribeOnTranslation([&](){
			_positionEdited = true;
		})),
		_rotationContract(owner()->transform()->subscribeOnRotation([&](){
			_positionEdited = true;
		})),
		_fov(90.0f),
		_size(20, 20),
		_aspectRatio(1.33f),
		_nearPlane(0.1f),
		_farPlane(1000.0f)
		
	{
		_updateProjectionMatrix();

		if (_mainCamera == nullptr)
			setAsMainCamera();
	}

	Camera::~Camera()
	{
		
	}

	spk::SafePointer<Camera> Camera::mainCamera()
	{
		return (spk::SafePointer<Camera>(_mainCamera));
	}

	void Camera::setAsMainCamera()
	{
		_mainCamera = this;
		_needUpdate = true;
	}

	void Camera::setType(Type p_type) {
		_type = p_type;
		_updateProjectionMatrix();
	}

	void Camera::setPerspectiveParameters(float p_fov, float p_aspectRatio, float p_nearPlane, float p_farPlane)
	{
		_fov = p_fov;
		_aspectRatio = p_aspectRatio;
		_nearPlane = p_nearPlane;
		_farPlane = p_farPlane;
		_updateProjectionMatrix();
	}

	void Camera::setOrthographicParameters(const spk::Vector2& p_size, float p_nearPlane, float p_farPlane)
	{
		_size = p_size;
		_nearPlane = p_nearPlane;
		_farPlane = p_farPlane;
		_updateProjectionMatrix();
	}

	const spk::Matrix4x4& Camera::MVP() const
	{
		return (_MVP);
	}
}