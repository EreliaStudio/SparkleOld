#pragma once

#include "system/spk_time_metrics.hpp"
#include "threading/spk_persistent_worker.hpp"

namespace spk
{
	/**
	 * @class Chronometer
	 * @brief Class for measuring the time span of an operation.
	 *
	 * This class represents a chronometer, which can be used to measure
	 * the duration of an operation in terms of time. This is achieved
	 * by recording the time at which the operation starts and the time
	 * at which it ends, and then calculating the difference between these two times.
	 */
	class Chronometer
	{
	private:
		/**
		 * @brief The time the chronometer started.
		 *
		 * This variable is used to record the time at which the chronometer starts.
		 * It is recorded in milliseconds since some fixed point in time (often system startup).
		 */
		long long _start;

		/**
		 * @brief The duration of the operation being timed.
		 *
		 * This variable is used to store the duration of the operation that the chronometer is timing.
		 * It is calculated as the difference between the current time and the start time.
		 */
		mutable long long _duration;

	public:
		/**
		 * @brief Construct a new Chronometer object
		 *
		 * This is the default constructor for the Chronometer class.
		 * It initializes the start time and the duration to zero.
		 */
		Chronometer();

		/**
		 * @brief Destroy the Chronometer object
		 *
		 * This is the destructor for the Chronometer class.
		 * It does not need to perform any specific action because the class does not allocate any dynamic resources.
		 */
		~Chronometer();

		/**
		 * @brief Starts the chronometer.
		 *
		 * This function starts the chronometer by recording the current time as the start time.
		 */
		void start();

		/**
		 * @brief Get the duration of the timed operation.
		 *
		 * This function returns the duration of the operation that the chronometer is timing.
		 *
		 * @return The duration of the timed operation in milliseconds.
		 */
		const long long &duration() const;

		/**
		 * @brief Stops the chronometer and calculates the duration of the timed operation.
		 *
		 * This function stops the chronometer by recording the current time, and then calculates
		 * the duration of the operation by subtracting the start time from this time.
		 *
		 * @return The duration of the timed operation in milliseconds.
		 */
		const long long &stop();

		/**
		 * @brief Check if the chronometer is running.
		 *
		 * This function checks if the chronometer is currently running by checking if the start time is not zero.
		 *
		 * @return True if the chronometer is running, false otherwise.
		 */
		bool isRunning() const { return (_start != 0); }
	};

}