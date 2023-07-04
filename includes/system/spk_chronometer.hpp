#pragma once

#include "system/spk_time_metrics.hpp"
#include "threading/spk_persistent_worker.hpp"

namespace spk
{
	class Chronometer
	{
	private:
		long long _start;
		mutable long long _duration;
		bool _isRunning;

	public:
		Chronometer();
		~Chronometer();

		/**
		 * @brief Start the chronometer and reset the duration
		 */
		void start();

		/**
		 * @brief 
		 *  Stop the duration 
		 */
		void pause();

		/**
		 * @brief 
		 *  Start the duration from the time it was put in pause
		 */
		void resume();
		
		/**
		 * @brief Get the duration since the start of the chronometer
		 * @return The duration in milliseconds
		 * @note Return 0 if the chronometer is not started.
		 * @note If stop has been called, the duration is not updated.
		 */
		const long long& duration() const;

		/**
		 * @brief Stop the chronometer and return the duration
		 * @return The duration in milliseconds
		 * @note Return 0 if the chronometer is not started
		 */
		const long long& stop();

		/**
		 * @brief Check if the chronometer is running
		 * @return True if the chronometer is running, false otherwise
		 */
		bool isRunning() const { return (_start != 0); }
	};
}