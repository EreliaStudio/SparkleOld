#include "engine/component/spk_transform_component.hpp"

namespace spk
{
	Transform::Transform(std::shared_ptr<GameObject> p_owner)
			: Component(p_owner), position(0, 0, 0), size(1, 1, 1), rotation(0, 0, 0) {}

	bool Transform::_onUpdate()
	{
		return (false);
	}

	void Transform::_onRender()
	{

	}

	spk::Vector3 Transform::calculateRotationFromVectors(const spk::Vector3& right, const spk::Vector3& up, const spk::Vector3& forward)
	{
		// /!\ Right hand axis /!\ 
		return(spk::Vector3(
			std::atan2(forward.x, forward.z),
			std::atan2(-forward.y, std::sqrt(forward.x * forward.x + forward.z * forward.z)),
			std::atan2(right.y, up.y)
		));
	}

	void Transform::lookAt(const spk::Vector3& target, const spk::Vector3& up)
	{
		spk::Vector3 forward = (target - position).normalize();
		spk::Vector3 right = up.cross(forward).normalize();
		spk::Vector3 newUp = forward.cross(right);

		rotation = calculateRotationFromVectors(right, newUp, forward);
	}
}