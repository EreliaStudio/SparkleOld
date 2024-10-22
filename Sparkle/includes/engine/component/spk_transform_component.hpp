#pragma once

#include "engine/component/spk_component.hpp"
#include "math/spk_vector3.hpp"
#include "design_pattern/spk_observable_value.hpp"

namespace spk
{
	/**
     * @brief Represents the transformation component in the Sparkle engine.
     *
     * This class manages the position, scale, and rotation of game objects.
     * It also provides functionalities to observe and react to changes in these properties.
     */
    class Transform : public Component
    {
    public:
        using TranslationType = ObservableValue<spk::Vector3>; ///< Type for translation observable.
        using ScaleType = ObservableValue<spk::Vector3>; ///< Type for scale observable.
        using RotationType = ObservableValue<spk::Vector3>; ///< Type for rotation observable.

	    protected:
        TranslationType _translation; ///< Observable value for position/translation.
        ScaleType _scale; ///< Observable value for scale.
        RotationType _rotation; ///< Observable value for rotation.

        spk::Vector3 _forward; ///< Forward direction vector.
        spk::Vector3 _right; ///< Right direction vector.
        spk::Vector3 _up; ///< Up direction vector.

        virtual bool _onUpdate() override;
        virtual void _onRender() override;

        void _computeDirections(); ///< Computes the directional vectors based on the current rotation.

    public:
        /**
         * @brief Constructs a new Transform object.
         */
        Transform();

        /**
         * @brief Orients the object to look at a specified target.
         *
         * @param p_target The target position to look at.
         * @param p_up The up direction, defaulting to a vector pointing upwards.
         */
        void lookAt(const spk::Vector3& p_target, const spk::Vector3& p_up = spk::Vector3(0, 1, 0));

		/**
		 * @brief Subscribes a function to be called when the translation changes.
		 *
		 * @param p_function The function to call on translation change.
		 * @return std::shared_ptr<TranslationType::Contract> A contract to manage the subscription.
		 */
		std::shared_ptr<TranslationType::Contract> subscribeOnTranslation(const std::function<void()> p_function);

		/**
		 * @brief Subscribes a function to be called when the scaling changes.
		 *
		 * @param p_function The function to call on scale change.
		 * @return std::shared_ptr<ScaleType::Contract> A contract to manage the subscription.
		 */
		std::shared_ptr<ScaleType::Contract> subscribeOnScaling(const std::function<void()> p_function);

		/**
		 * @brief Subscribes a function to be called when the rotation changes.
		 *
		 * @param p_function The function to call on rotation change.
		 * @return std::shared_ptr<RotationType::Contract> A contract to manage the subscription.
		 */
		std::shared_ptr<RotationType::Contract> subscribeOnRotation(const std::function<void()> p_function);

		/**
		 * @brief Sets the translation of the object.
		 *
		 * @param p_newTranslation The new translation vector.
		 */
		void place(const spk::Vector3& p_newTranslation);

		/**
		 * @brief Moves the object by a given delta translation.
		 *
		 * @param p_deltaTranslation The delta vector to move the object.
		 */
		void move(const spk::Vector3& p_deltaTranslation);

		/**
		 * @brief Gets the current translation of the object.
		 *
		 * @return const spk::Vector3& The current translation vector.
		 */
		const spk::Vector3& translation() const;

		/**
		 * @brief Sets the scale of the object.
		 *
		 * @param p_newScale The new scale vector.
		 */
		void setScale(const spk::Vector3& p_newScale);

		/**
		 * @brief Applies a scaling factor to the object.
		 *
		 * @param p_scale The scale vector to apply.
		 */
		void applyScaling(const spk::Vector3& p_scale);

		/**
		 * @brief Gets the current scale of the object.
		 *
		 * @return const spk::Vector3& The current scale vector.
		 */
		const spk::Vector3& scale() const;

		/**
		 * @brief Sets the rotation of the object.
		 *
		 * @param p_rotation The new rotation vector.
		 */
		void setRotation(const spk::Vector3& p_rotation);

		/**
		 * @brief Rotates the object by a given delta rotation.
		 *
		 * @param p_deltaRotation The delta rotation vector to apply.
		 */
		void rotate(const spk::Vector3& p_deltaRotation);

		/**
		 * @brief Gets the current rotation of the object.
		 *
		 * @return const spk::Vector3& The current rotation vector.
		 */
		const spk::Vector3& rotation() const;

		/**
		 * @brief Gets the forward direction vector of the object.
		 *
		 * The forward vector represents the direction in which the object is facing.
		 * This is typically used in camera and movement calculations.
		 * 
		 * @return const spk::Vector3& The forward direction vector.
		 */
		const spk::Vector3& forward() const;

		/**
		 * @brief Gets the right direction vector of the object.
		 *
		 * The right vector represents the direction to the right side of the object,
		 * perpendicular to the forward vector. Useful for movement and orientation.
		 * 
		 * @return const spk::Vector3& The right direction vector.
		 */
		const spk::Vector3& right() const;

		/**
		 * @brief Gets the up direction vector of the object.
		 *
		 * The up vector represents the upward direction relative to the object,
		 * typically used in camera orientation and determining the top side of the object.
		 * 
		 * @return const spk::Vector3& The up direction vector.
		 */
		const spk::Vector3& up() const;
	};
}