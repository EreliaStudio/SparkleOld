#pragma once

#include "engine/component/spk_component.hpp"
#include "math/spk_vector3.hpp"
#include "design_pattern/spk_value.hpp"

namespace spk
{
	class Transform : public Component
	{
	public:
		using TranslationType = ValueWrapper<spk::Vector3>;
		using ScaleType = ValueWrapper<spk::Vector3>;
		using RotationType = ValueWrapper<spk::Vector3>;

	protected:

		static inline TranslationType::Default _defaultPosition = 0;
		TranslationType _translation;

		static inline ScaleType::Default _defaultSize = 0;
		ScaleType _scale;

		static inline RotationType::Default _defaultRotation = 0;
		RotationType _rotation;

		spk::Vector3 _forward;
		spk::Vector3 _right;
		spk::Vector3 _up;
		
		virtual bool _onUpdate() override;
		virtual void _onRender() override;

		spk::Vector3 _calculateRotationFromVectors(const spk::Vector3& p_right, const spk::Vector3& p_up, const spk::Vector3& p_forward);

		void _computeDirections();

	public:

		Transform(std::shared_ptr<GameObject> p_owner);

		void lookAt(const spk::Vector3& p_target, const spk::Vector3& p_up = spk::Vector3(0, 1, 0));

		TranslationType::Contract subscribeOnTranslation(const std::function<void()> p_function);
		ScaleType::Contract subscribeOnScaling(const std::function<void()> p_function);
		RotationType::Contract subscribeOnRotation(const std::function<void()> p_function);

		void place(const spk::Vector3& p_newPosition);
		void move(const spk::Vector3& p_translation);
		const spk::Vector3& translation() const;

		void setScale(const spk::Vector3& p_newScale);
		void applyScaling(const spk::Vector3& p_scale);
		const spk::Vector3& scale() const;

		void setRotation(const spk::Vector3& p_rotation);
		void rotate(const spk::Vector3& p_deltaRotation);
		const spk::Vector3& rotation() const;

		const spk::Vector3& forward() const;
		const spk::Vector3& right() const;
		const spk::Vector3& up() const;
	};
}