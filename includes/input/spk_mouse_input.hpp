#pragma once

#include "input/spk_input.h"
#include "input/spk_mouse.h"

namespace spk
{
	/**
	 * @class MouseInput
	 * 
	 * @brief A concrete class derived from Input that represents mouse input.
	 * 
	 * MouseInput is a specific implementation of Input for mouse inputs. It encapsulates a mouse button and the expected status of the button.
	 * The encapsulated function is executed if the current status of the specified mouse button matches the expected status.
	 */
	class MouseInput : public Input
	{
	private:
		/**
		 * @brief The mouse button that this MouseInput object is monitoring.
		 */
		spk::Mouse::Button _button;
		
		/**
		 * @brief The expected status of the mouse button.
		 */
		spk::InputStatus _expectedStatus;

		/**
		 * @brief Checks whether the function execution is needed.
		 * 
		 * The function execution is needed if the current status of the specified mouse button matches the expected status.
		 * 
		 * @return true if the function execution is needed, false otherwise.
		 */
		bool _isExecutionNeeded() {
			return (spk::Mouse::instance()->getButton(_button) == _expectedStatus);
		}

	public:
		/**
		 * @brief Construct a new MouseInput object.
		 * 
		 * @tparam Funct Function type.
		 * @tparam Args Argument types.
		 * @param p_button The mouse button that this MouseInput object should monitor.
		 * @param p_expectedStatus The expected status of the mouse button.
		 * @param p_delayBetweenInput Input delay for the timer.
		 * @param p_funct Function to be executed.
		 * @param p_args Arguments for the function.
		 */
		template <typename Funct, typename... Args>
		MouseInput(spk::Mouse::Button p_button, spk::InputStatus p_expectedStatus, unsigned long p_delayBetweenInput, Funct&& p_funct, Args&&... p_args) :
			Input(p_delayBetweenInput, std::forward<Funct>(p_funct), std::forward<Args>(p_args)...),
			_button(p_button),
			_expectedStatus(p_expectedStatus)
		{

		}
	};
}
