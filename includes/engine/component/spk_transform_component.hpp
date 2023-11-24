#pragma once

#include "engine/component/spk_component.hpp"
#include "math/spk_vector3.hpp"
#include "design_pattern/spk_observable_value.hpp"

namespace spk
{
	class Transform : public Component
	{
	public:
		using TranslationType = ObservableValue<spk::Vector3>;
		using ScaleType = ObservableValue<spk::Vector3>;
		using RotationType = ObservableValue<spk::Vector3>;

	protected:
		TranslationType _translation;
		ScaleType _scale;
		RotationType _rotation;

		spk::Vector3 _forward;
		spk::Vector3 _right;
		spk::Vector3 _up;
		
		virtual bool _onUpdate() override;
		virtual void _onRender() override;

		void _computeDirections();

	public:
		Transform();

		void lookAt(const spk::Vector3& p_target, const spk::Vector3& p_up = spk::Vector3(0, 1, 0));

		std::shared_ptr<TranslationType::Contract> subscribeOnTranslation(const std::function<void()> p_function);
		std::shared_ptr<ScaleType::Contract> subscribeOnScaling(const std::function<void()> p_function);
		std::shared_ptr<RotationType::Contract> subscribeOnRotation(const std::function<void()> p_function);

		void place(const spk::Vector3& p_newTranslation);
		void move(const spk::Vector3& p_deltaTranslation);
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