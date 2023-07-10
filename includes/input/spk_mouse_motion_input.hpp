#pragma once

#include "input/spk_input.h"
#include "input/spk_mouse.h"

namespace spk
{
	/**
	 * @class MouseMovementInput
	 * 
	 * @brief A concrete class derived from IInput that represents mouse movement input.
	 * 
	 * MouseMovementInput is a specific implementation of IInput for mouse movements. 
	 * The encapsulated function is always executed in every update, because the `_isExecutionNeeded()` method always returns true.
	 */
	class MouseMovementInput : public IInput
	{
	private:
		/**
		 * @brief Checks whether the function execution is needed.
		 * 
		 * In the case of MouseMovementInput, the function execution is always needed.
		 * 
		 * @return Always returns true.
		 */
		bool _isExecutionNeeded() {
			return (true);
		}

	public:
		/**
		 * @brief Construct a new MouseMovementInput object.
		 * 
		 * @tparam Funct Function type.
		 * @tparam Args Argument types.
		 * @param p_delayBetweenInput Input delay for the timer.
		 * @param p_funct Function to be executed.
		 * @param p_args Arguments for the function.
		 */
		template <typename Funct, typename... Args>
		MouseMovementInput(unsigned long p_delayBetweenInput, Funct&& p_funct, Args&&... p_args) :
			IInput(p_delayBetweenInput, std::forward<Funct>(p_funct), std::forward<Args>(p_args)...)
		{

		}
	};
}
