#include "engine/component/spk_transform_component.hpp"

namespace spk
{
	Transform::Transform(std::shared_ptr<GameObject> p_owner) :
		Component(p_owner, L"Transform"),
		_translation(std::shared_ptr<TranslationType::Default>(&_defaultRotation, [](TranslationType::Default*){})),
		_scale(std::shared_ptr<ScaleType::Default>(&_defaultRotation, [](ScaleType::Default*){})),
		_rotation(std::shared_ptr<RotationType::Default>(&_defaultRotation, [](RotationType::Default*){})),
		_forward(1, 0, 0),
		_right(0, 1, 0),
		_up(0, 0, 1)
	{
		
	}
	
	void Transform::_computeDirections()
	{
		float pitch = _rotation.get().x * (static_cast<float>(M_PI) / 180.0f);
		float yaw = _rotation.get().y * (static_cast<float>(M_PI) / 180.0f);
		float roll = _rotation.get().z * (static_cast<float>(M_PI) / 180.0f);

		_forward.x = cos(yaw) * cos(pitch);
		_forward.y = sin(pitch);
		_forward.z = sin(yaw) * cos(pitch);
		_forward.normalize();

		spk::Vector3 globalUp = spk::Vector3(0, 1, 0);
		_right = globalUp.cross(_forward);
		_right.normalize();

		_up = _forward.cross(_right);
		_up.normalize();

		_rotation.resetUpdateFlag();
	}

	bool Transform::_onUpdate()
	{
		if (_rotation.needUpdate() == true)
			_computeDirections();

		return (false);
	}

	void Transform::_onRender()
	{

	}

	spk::Vector3 Transform::_calculateRotationFromVectors(const spk::Vector3& p_right, const spk::Vector3& p_up, const spk::Vector3& p_forward)
	{
		return(spk::Vector3(
			std::atan2(_forward.x, _forward.z),
			std::atan2(-_forward.y, std::sqrt(_forward.x * _forward.x + _forward.z * _forward.z)),
			std::atan2(_right.y, _up.y)
		));
	}

	void Transform::lookAt(const spk::Vector3& p_target, const spk::Vector3& p_up)
	{
		_forward = (p_target - _translation).normalize();
		_right = p_up.cross(_forward).normalize();
		_up = _forward.cross(_right);

		_rotation = _calculateRotationFromVectors(_right, _up, _forward);
	}
	
	Transform::TranslationType::Contract Transform::subscribeOnTranslation(const std::function<void()> p_function)
	{
		return (std::move(_translation.value().subscribe(p_function)));
	}
	
	Transform::ScaleType::Contract Transform::subscribeOnScaling(const std::function<void()> p_function)
	{
		return (std::move(_scale.value().subscribe(p_function)));
	}
	
	Transform::RotationType::Contract Transform::subscribeOnRotation(const std::function<void()> p_function)
	{
		return (std::move(_rotation.value().subscribe(p_function)));
	}

	void Transform::place(const spk::Vector3& p_newPosition)
	{
		_translation = p_newPosition;
	}
	
	void Transform::move(const spk::Vector3& p_translation)
	{
		_translation = _translation.get() + p_translation;
	}
	
	const spk::Vector3& Transform::translation() const
	{
		return (_translation.get());
	}

	void Transform::setScale(const spk::Vector3& p_newScale)
	{
		_scale = p_newScale;
	}
	
	void Transform::applyScaling(const spk::Vector3& p_scale)
	{
		_scale = _scale.get() + p_scale;
	}
	
	const spk::Vector3& Transform::scale() const
	{
		return (_scale.get());
	}

	void Transform::setRotation(const spk::Vector3& p_rotation)
	{
		_rotation = p_rotation;
	}

	void Transform::rotate(const spk::Vector3& p_deltaRotation)
	{
		_rotation = _rotation.get() + p_deltaRotation;
	}

	const spk::Vector3& Transform::rotation() const
	{
		return (_rotation.get());
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