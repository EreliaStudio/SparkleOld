#pragma once

#include <stdexcept>
#include "system/spk_time_metrics.hpp"

namespace spk
{
	/**
	 * @class Timer
	 * @brief Class for creating and managing a timer.
	 * 
	 * This class allows you to create a timer with a specified duration. 
	 * The timer can be started, stopped, and checked for its status.
	 */
	class Timer
	{
	public:
		/**
		 * @enum State
		 * @brief An enum class to represent the state of the timer.
		 * 
		 * This enum class has three states: Idle, Running, and Timeout. 
		 * The timer is in the Idle state by default, changes to Running when it starts, 
		 * and goes to Timeout when the duration has passed.
		 */
		enum class State
		{
			Idle,	///< Timer is idle (not running and not timed out).
			Running, ///< Timer is currently running.
			Timeout, ///< Timer has timed out.
		};

	private:
		/**
		 * @brief The duration of the timer in milliseconds.
		 * 
		 * This variable is used to store the duration of the timer, 
		 * which is set by the user when creating the timer.
		 */
		long long _duration;

		/**
		 * @brief The current state of the timer.
		 * 
		 * This variable is used to store the current state of the timer, 
		 * which can be Idle, Running, or Timeout.
		 */
		mutable State _status;

		/**
		 * @brief The time the timer started.
		 * 
		 * This variable is used to record the time at which the timer starts. 
		 * It is recorded in milliseconds since some fixed point in time (often system startup).
		 */
		long long _startTime;

	public:
		/**
		 * @brief Construct a new Timer object.
		 * 
		 * This is the default constructor for the Timer class. 
		 * It initializes the duration and start time to zero, and the status to Idle.
		 * 
		 * @param p_duration The duration of the timer in milliseconds.
		 */
		Timer(const long long& p_duration = 0);

		const long long& duration() const;

		/**
		 * @brief Sets the duration of the timer.
		 * 
		 * This function sets the duration of the timer to the specified value.
		 * 
		 * @param p_duration The duration of the timer in milliseconds.
		 */
		void setDuration(const long long& p_duration);

		/**
		 * @brief Get the total duration of the timer.
		 * 
		 * This function returns the total duration of the timer.
		 * 
		 * @return The total duration of the timer in milliseconds.
		 */
		const long long& totalDuration() const;

		/**
		 * @brief Get the remaining duration of the timer.
		 * 
		 * This function returns the remaining duration of the timer, 
		 * calculated as the total duration minus the elapsed time since the timer started.
		 * 
		 * @return The remaining duration of the timer in milliseconds.
		 */
		long long remainingDuration() const;

		/**
		 * @brief Starts the timer.
		 * 
		 * This function starts the timer by recording the current time as the start time and 
		 * changing the status to Running.
		 */
		void start();

		/**
		 * @brief Stops the timer.
		 * 
		 * This function stops the timer and changes the status to Idle.
		 */
		void stop();

		/**
		 * @brief Get the status of the timer.
		 * 
		 * This function returns the current status of the timer, which can be Idle, Running, or Timeout.
		 * 
		 * @return The current status of the timer.
		 */
		const State& status() const;

		/**
		 * @brief Check if thetimer is running.

		 * This function checks if the timer is currently running by checking if the status is Running.
		 * 
		 * @return True if the timer is running, false otherwise.
		 */
		bool isRunning() const;
	};
}
