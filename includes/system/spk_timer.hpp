#pragma once

#include <stdexcept>
#include "system/spk_time_metrics.hpp"

namespace spk
{
	class Timer
	{
	public:
		enum class State
		{
			Idle,
			Running,
			Timeout,
		};

	private:
		long long _duration;
		mutable State _status;

		long long _startTime;

	public:
		Timer();

		/**
		 * @brief Set the Duration object
		 *
		 * @param p_duration
		 * @throw std::runtime_error if the timer is running
		 */
		void setDuration(const long long& p_duration);

		const long long& totalDuration() const;

		/**
		 * @brief Get the remaining duration
		 * 
		 * @return long long How much time is left before the timer times out
		 * @note If the timer is timed out, returns for how long it has been timed out
		 */
		long long remainingDuration() const;

		void start();
		void stop();

		const State& status() const;
		bool isRunning() const;
	};
}