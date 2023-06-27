#pragma once

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
	public:
		Timer();

		/**
		 * @brief Set the Duration object
		 *
		 * @param p_duration
		 * @throw std::runtime_error if the timer is running
		 */
		void setDuration(const long long& p_duration);

		const long long& duration() const;
		long long remainingDuration() const;

		void start();
		void stop();

		const State& status() const;
		bool isRunning() const;
	};
}