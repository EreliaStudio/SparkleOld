#pragma once

#include "design_pattern/spk_stated_object.hpp"

namespace spk
{
	/**
	 * @brief Represents an activatable object.
	 * This class provides functionality to activate and deactivate an object.
	 * It also allows adding activation and deactivation callbacks.
	 */
	class ActivableObject : protected StatedObject<bool>
	{
	public:
		using Contract = CallbackContainer::Contract;
		using Callback = CallbackContainer::Callback;

	private:

	public:
		/**
		 * @brief Constructs an ActivableObject object.
		 * @param p_activate Indicates whether the object should be activated upon construction. Default is false.
		 */
		ActivableObject(bool p_activate = false);
		/**
		 * @brief Activates the object.
		 * This function activates the object.
		 */
		void activate();

		/**
		 * @brief Deactivates the object.
		 * This function deactivates the object.
		 */
		void deactivate();

		/**
		 * @brief Checks if the object is active.
		 * @return A const reference to a boolean indicating whether the object is active or not.
		 */
		const bool& isActive() const;

		/**
		 * @brief Adds a callback to be invoked when the object is activated.
		 *
		 * This method allows the registration of a callback function that will be called whenever
		 * the object transitions from an inactive to an active state. This can be useful for triggering
		 * specific actions or notifications upon the object's activation.
		 *
		 * @param p_callback A function to be called when the object is activated. The function should match
		 *                   the signature defined in Callback.
		 * @return Contract A contract object that represents the registration of the callback.
		 *         This contract can be used to manage the callback, such as removing it at a later time.
		 */
		Contract addActivationCallback(const Callback& p_callback);

		/**
		 * @brief Adds a callback to be invoked when the object is deactivated.
		 *
		 * Similar to addActivationCallback, this method registers a callback function that is called
		 * when the object transitions from an active to an inactive state. It is useful for handling
		 * actions that need to occur upon the deactivation of the object.
		 *
		 * @param p_callback A function to be called when the object is deactivated. The function should conform
		 *                   to the signature specified in Callback.
		 * @return Contract A contract object that signifies the callback's registration.
		 *         This contract is useful for managing the lifecycle of the callback, including its removal if needed.
		 */
		Contract addDeactivationCallback(const Callback& p_callback);
	};
}
