#pragma once

#include "system/spk_time_metrics.hpp"
#include "threading/spk_persistent_worker.hpp"

namespace spk
{
	class Chronometer
	{
	private:
		long long _start;
		long long _duration;

	public:
		Chronometer();
		~Chronometer();

		/**
		 * @brief Start the chronometer and reset the duration
		 */
		void start();
		
		/**
		 * @brief Get the duration since the start of the chronometer
		 * @return The duration in milliseconds
		 * @note Return 0 if the chronometer is not started.
		 * @note If stop has been called, the duration is not updated.
		 */
		const long long& duration();

		/**
		 * @brief Stop the chronometer and return the duration
		 * @return The duration in milliseconds
		 * @note Return 0 if the chronometer is not started
		 */
		const long long& stop();
	};
}