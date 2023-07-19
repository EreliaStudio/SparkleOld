#pragma once

#include <functional>
#include "system/spk_timer.hpp"

namespace spk
{
	/**
	 * @brief Represents an activity.
	 * This class defines an activity that can be managed by an ActivityManager.
	 * It provides methods for updating, moving to the next step, starting, and checking the status of the activity.
	 */
	class Activity
	{
		friend class ActivityManager;

	public:
		/**
		 * @brief Enumerates the possible status of the activity.
		 */
		enum class Status
		{
			Stopped,   /**< The activity is stopped. */
			OnEnter,   /**< The activity is in the onEnter phase. */
			Running,   /**< The activity is running. */
			OnExit,	/**< The activity is in the onExit phase. */
			Complete   /**< The activity is complete. */
		};

		Status _internalWorkingState; /**< The internal working state of the activity. */

		/**
		 * @brief Updates the activity.
		 * This function updates the activity, typically called once per frame.
		 * It is responsible for executing the activity's current step and managing the state transitions.
		 */
		void _update();

		/**
		 * @brief Moves to the next step of the activity.
		 * This function moves the activity to the next step without executing it.
		 * It is used for advancing the activity's progress without performing any actions.
		 */
		void _moveNextStep();

		/**
		 * @brief Moves to the next step of the activity and executes it.
		 * This function moves the activity to the next step and executes it.
		 * It is used for advancing the activity's progress and performing the associated actions.
		 */
		void _moveNextStepAndExecute();

		/**
		 * @brief Starts the activity.
		 * This function starts the activity, setting its initial state and entering the onEnter phase.
		 * It is called when the activity is initially activated.
		 */
		void _start();

	protected:
		/**
		 * @brief Executes the onEnter phase of the activity.
		 * This pure virtual function is called when the activity enters the onEnter phase.
		 * Subclasses must override this function to provide custom implementation.
		 */
		virtual void _onEnter() = 0;

		/**
		 * @brief Executes the main execution phase of the activity.
		 * This pure virtual function is called when the activity is in the running state and needs to execute its main logic.
		 * Subclasses must override this function to provide custom implementation.
		 */
		virtual void _execute() = 0;

		/**
		 * @brief Executes the onExit phase of the activity.
		 * This pure virtual function is called when the activity enters the onExit phase.
		 * Subclasses must override this function to provide custom implementation.
		 */
		virtual void _onExit() = 0;

	public:
		/**
		 * @brief Default constructor for Activity.
		 * This constructor initializes the internal working state of the activity.
		 */
		Activity();

		/**
		 * @brief Returns the current status of the activity.
		 * @return A const reference to the current status of the activity.
		 */
		const Status& state() const;

		/**
		 * @brief Checks if the activity is completed.
		 * @return True if the activity is complete, false otherwise.
		 */
		bool isCompleted() const;
	};
	
	/**
	 * @class TimedActivity
	 * @brief Represents an activity that executes a function after a specified delay.
	 *
	 * TimedActivity is an Activity that begins a timer on activation. Once the timer
	 * has reached the specified delay, it executes a specified function.
	 * This is useful for scheduling tasks to run after a certain amount of time has passed.
	 */
	class TimedActivity : public spk::Activity
	{
	private:
		/// Timer object used to keep track of time for this activity.
		spk::Timer _timer;

		/// Function to be executed by the timed activity.
		std::function<void()> _funct;

		/// The onEnter function that is called when the activity enters the onEnter state.
		void _onEnter();

		/// The execution function that is called when the activity is in the running state.
		void _execute();

		/// The onExit function that is called when the activity enters the onExit state.
		void _onExit();

	public:
		/// Default constructor for TimedActivity.
		TimedActivity();

		/**
		 * @brief Constructs a TimedActivity with a delay and a function.
		 *
		 * @param p_delay The delay before the function should be executed.
		 * @param p_funct The function to be executed.
		 */
		TimedActivity(const size_t& p_delay, const std::function<void()>& p_funct);

		/**
		 * @brief Sets the function to be executed by the TimedActivity.
		 *
		 * @param p_funct The function to be executed.
		 */
		void setFunct(const std::function<void()>& p_funct);

		/**
		 * @brief Sets the delay before the function should be executed.
		 *
		 * @param p_delay The delay before the function should be executed.
		 */
		void setDelay(const size_t& p_delay);
	};

	/**
	 * @class RetryActivity
	 * @brief Represents an activity that repeatedly executes a function until it returns true.
	 *
	 * RetryActivity is an Activity that continuously retries a specified function until it returns true.
	 * This is useful for tasks that might fail and need to be retried several times until they succeed.
	 */
	class RetryActivity : public spk::Activity
	{
	private:
		/// Function to be executed by the RetryActivity.
		std::function<bool()> _funct;

		/// The onEnter function that is called when the activity enters the onEnter state.
		void _onEnter();

		/// The execution function that is called when the activity is in the running state.
		void _execute();

		/// The onExit function that is called when the activity enters the onExit state.
		void _onExit();

	public:
		/// Default constructor for RetryActivity.
		RetryActivity();

		/**
		 * @brief Constructs a RetryActivity with a function.
		 *
		 * @param p_funct The function to be executed.
		 */
		RetryActivity(const std::function<bool()>& p_funct);

		/**
		 * @brief Sets the function to be executed by the RetryActivity.
		 *
		 * @param p_funct The function to be executed.
		 */
		void setFunct(const std::function<bool()>& p_funct);
	};

}
