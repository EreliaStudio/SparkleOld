#pragma once

#include "engine/component/spk_component.hpp"
#include "math/spk_vector3.hpp"

namespace spk
{
	class Transform : public Component
	{
	protected:
		virtual bool _onUpdate() override;
		virtual void _onRender() override;

		spk::Vector3 calculateRotationFromVectors(const spk::Vector3& right, const spk::Vector3& up, const spk::Vector3& forward);

	public:
		spk::Vector3 position;
		spk::Vector3 size;
		spk::Vector3 rotation;

		Transform(std::shared_ptr<GameObject> p_owner);

		void lookAt(const spk::Vector3& target, const spk::Vector3& up = spk::Vector3(0, 1, 0));
	};
}