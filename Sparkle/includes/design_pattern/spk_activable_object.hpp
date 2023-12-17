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
	private:
		// Private members (not specified in the code snippet)

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

		CallbackContainer::Contract addActivationCallback(const CallbackContainer::Callback& p_callback);

		CallbackContainer::Contract addDeactivationCallback(const CallbackContainer::Callback& p_callback);
	};
}
