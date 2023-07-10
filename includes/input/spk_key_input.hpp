#pragma once

#include "input/spk_input.h"
#include "input/spk_keyboard.h"

namespace spk
{
	/**
	 * @class KeyInput
	 * 
	 * @brief A concrete class derived from IInput that represents keyboard input.
	 * 
	 * KeyInput is a specific implementation of IInput for keyboard inputs. It encapsulates a keyboard key and the expected status of the key.
	 * The encapsulated function is executed if the current status of the specified keyboard key matches the expected status.
	 */
	class KeyInput : public IInput
	{
	private:
		/**
		 * @brief The keyboard key that this KeyInput object is monitoring.
		 */
		spk::Keyboard::Key _key;
		
		/**
		 * @brief The expected status of the keyboard key.
		 */
		spk::InputStatus _expectedStatus;

		/**
		 * @brief Checks whether the function execution is needed.
		 * 
		 * The function execution is needed if the current status of the specified keyboard key matches the expected status.
		 * 
		 * @return true if the function execution is needed, false otherwise.
		 */
		bool _isExecutionNeeded() {
			return (spk::Keyboard::instance()->getKey(_key) == _expectedStatus);
		}

	public:
		/**
		 * @brief Construct a new KeyInput object.
		 * 
		 * @tparam Funct Function type.
		 * @tparam Args Argument types.
		 * @param p_key The keyboard key that this KeyInput object should monitor.
		 * @param p_expectedStatus The expected status of the keyboard key.
		 * @param p_delayBetweenInput Input delay for the timer.
		 * @param p_funct Function to be executed.
		 * @param p_args Arguments for the function.
		 */
		template <typename Funct, typename... Args>
		KeyInput(spk::Keyboard::Key p_key, spk::InputStatus p_expectedStatus, unsigned long p_delayBetweenInput, Funct&& p_funct, Args&&... p_args) :
			IInput(p_delayBetweenInput, std::forward<Funct>(p_funct), std::forward<Args>(p_args)...),
			_key(p_key),
			_expectedStatus(p_expectedStatus)
		{

		}
	};
}
