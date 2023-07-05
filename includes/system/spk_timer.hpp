#pragma once

#include <stdexcept>
#include "system/spk_time_metrics.hpp"

namespace spk
{
    /**
     * \class Timer
     * \brief A class to manage time-based operations.
     *
     * This class provides an interface for a simple timer, which can be set to a specific duration, 
     * started, stopped, and queried for its current state and remaining duration.
     */
	class Timer
	{
	public:

        /**
         * \enum State
         * \brief Enumeration of the possible states for the Timer class.
         */
		enum class State
		{
			Idle,    ///< Timer is idle (not running and not timed out).
			Running, ///< Timer is currently running.
			Timeout, ///< Timer has timed out.
		};

	private:
		long long _duration; ///< The total duration for which the timer will run.
		mutable State _status; ///< The current state of the timer.
		long long _startTime; ///< The start time of the timer.

	public:
		/**
		 * \brief Default constructor for Timer class.
		 */
		Timer();

		/**
		 * \brief Set the duration for the Timer.
		 *
		 * \param p_duration The desired duration for the Timer.
		 * \throw std::runtime_error if the timer is currently running.
		 */
		void setDuration(const long long& p_duration);

		/**
		 * \brief Accessor function for the total duration of the Timer.
		 * \return A const reference to the total duration of the Timer.
		 */
		const long long& totalDuration() const;

		/**
		 * \brief Get the remaining duration of the Timer.
		 * 
		 * \return long long The amount of time left before the Timer times out.
		 * \note If the Timer has already timed out, this function returns how long it has been in the Timeout state.
		 */
		long long remainingDuration() const;

		/**
		 * \brief Start the Timer.
		 */
		void start();

		/**
		 * \brief Stop the Timer.
		 */
		void stop();

		/**
		 * \brief Accessor function for the current state of the Timer.
		 * \return A const reference to the current state of the Timer.
		 */
		const State& status() const;

		/**
		 * \brief Check if the Timer is currently running.
		 * \return bool True if the Timer is running, false otherwise.
		 */
		bool isRunning() const;
	};
}
