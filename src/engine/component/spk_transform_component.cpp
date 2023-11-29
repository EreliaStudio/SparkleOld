#include "engine/component/spk_transform_component.hpp"
#include "engine/spk_game_object.hpp"

namespace spk
{
	Transform::Transform() :
		Component(L"Transform"),
		_translation(0, 0, 0),
		_scale(1, 1, 1),
		_rotation(0, 0, 0)
	{
		_forward = spk::Vector3(0, 0, 1);
		_right = spk::Vector3(1, 0, 0);
		_up = spk::Vector3(0, 1, 0);
		_computeDirections();
	}
	
	void Transform::_computeDirections()
	{
		float yaw = spk::degreeToRadian(-_rotation.value().y);
        float pitch = spk::degreeToRadian(_rotation.value().x);

        _forward.x = cos(yaw) * cos(pitch);
        _forward.y = sin(pitch);
        _forward.z = sin(yaw) * cos(pitch);

		_right = _forward.cross(spk::Vector3(0, -1, 0));

		_up = _forward.cross(_right);
	}

	bool Transform::_onUpdate()
	{
		return (false);
	}

	void Transform::_onRender()
	{

	}

	void Transform::lookAt(const spk::Vector3& p_target, const spk::Vector3& p_up)
	{
		_forward = (p_target - _translation).normalize();
		_right = p_up.cross(_forward).normalize();
		_up = _forward.cross(_right);

		_rotation = spk::Vector3(
				spk::radianToDegree(std::atan2(-_forward.z, std::sqrt(_forward.x * _forward.x + _forward.y * _forward.y))),
				spk::radianToDegree(std::atan2(_forward.y, _forward.x)),
				spk::radianToDegree(std::atan2(_right.z, _up.z))
			);
	}

	std::shared_ptr<Transform::TranslationType::Contract> Transform::subscribeOnTranslation(const std::function<void()> p_function)
	{
		return (_translation.subscribe(p_function));
	}
	
	std::shared_ptr<Transform::ScaleType::Contract> Transform::subscribeOnScaling(const std::function<void()> p_function)
	{
		return (_scale.subscribe(p_function));
	}
	
	std::shared_ptr<Transform::RotationType::Contract> Transform::subscribeOnRotation(const std::function<void()> p_function)
	{
		return (_rotation.subscribe(p_function));
	}

	void Transform::place(const spk::Vector3& p_newTranslation)
	{
		_translation = p_newTranslation;
	}
	
	void Transform::move(const spk::Vector3& p_deltaTranslation)
	{
		_translation += p_deltaTranslation;
	}
	
	const spk::Vector3& Transform::translation() const
	{
		return (_translation);
	}

	void Transform::setScale(const spk::Vector3& p_newScale)
	{
		_scale = p_newScale;
	}
	
	void Transform::applyScaling(const spk::Vector3& p_scale)
	{
		_scale = _scale + p_scale;
	}
	
	const spk::Vector3& Transform::scale() const
	{
		return (_scale);
	}

	void Transform::setRotation(const spk::Vector3& p_rotation)
	{
		_rotation = p_rotation;
		_computeDirections();
	}

	void Transform::rotate(const spk::Vector3& p_deltaRotation)
	{
		if (p_deltaRotation == 0)
			return ;
			
		spk::Vector3 tmp = _rotation + p_deltaRotation;

		tmp.x = std::clamp(tmp.x, -89.5f, 89.5f);
		// while (tmp.y > 360)
		// 	tmp.y -= 360;
		// while (tmp.y < -360)
		// 	tmp.y += 360;

		_rotation = tmp;
		_computeDirections();
	}

	const spk::Vector3& Transform::rotation() const
	{
		return (_rotation);
	}

	const spk::Vector3& Transform::forward() const
	{
		return (_forward);
	}
	
	const spk::Vector3& Transform::right() const
	{
		return (_right);
	}
	
	const spk::Vector3& Transform::up() const
	{
		return (_up);
	}
}