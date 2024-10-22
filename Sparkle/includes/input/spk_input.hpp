#pragma once

#include <functional>
#include "system/spk_timer.hpp"

namespace spk
{
	/**
	 * @class Input
	 * 
	 * @brief Abstract class providing a common interface for input classes in the 'spk' library.
	 * 
	 * An Input object encapsulates a function that may need to be executed at a regular interval.
	 * The function is executed if the `_isExecutionNeeded()` method returns true.
	 * The `_skipCondition()` method can be overridden to provide a condition under which the update is skipped.
	 */
	class Input
	{
	protected:
		/**
		 * @brief Timer used to control the execution of the function.
		 */
		spk::Timer _timer;
		
		/**
		 * @brief The function to be executed.
		 */
		std::function<void()> _funct;

		/**
		 * @brief Pure virtual function to determine whether the function execution is needed.
		 * 
		 * @return true if function execution is needed, false otherwise.
		 */
		virtual bool _isExecutionNeeded() = 0;

		/**
		 * @brief Virtual function to provide a condition under which the update is skipped.
		 * 
		 * @return true by default. Override this function to provide a custom condition.
		 */
		virtual bool _skipCondition() {
			return (true);
		}

	public:
		/**
		 * @brief Construct a new Input object
		 * 
		 * @tparam Funct Function type
		 * @tparam Args Argument types
		 * @param p_delayBetweenInput Input delay for the timer
		 * @param p_funct Function to be executed
		 * @param p_args Arguments for the function
		 */
		template <typename Funct, typename... Args>
		Input(unsigned long p_delayBetweenInput, Funct&& p_funct, Args&&... p_args) :
			_timer(p_delayBetweenInput),
			_funct(std::bind(std::forward<Funct>(p_funct), std::forward<Args>(p_args)...))
		{

		}

		/**
		 * @brief Update method for the Input object.
		 * 
		 * If the `_skipCondition()` is true and the timer is running, the update is skipped.
		 * If the `_isExecutionNeeded()` method returns true, the function is executed and the timer is started.
		 */
		virtual void update()
		{
			if (_skipCondition() == true && _timer.isRunning())
				return;

			if (_isExecutionNeeded() == true) {
				_funct();
				if (_timer.duration() != 0)
					_timer.start();
			}
		}
	};
}
