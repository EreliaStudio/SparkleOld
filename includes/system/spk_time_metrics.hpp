#pragma once

#include "design_pattern/spk_singleton.hpp"
#include "application/modules/spk_time_module.hpp"


namespace spk
{
	/**
	 * @class TimeMetrics
	 * @brief Class for recording time-related metrics in the application.
	 *
	 * This class is a singleton and can be accessed anywhere in the application to get time-related metrics. 
	 * It keeps track of two time metrics: the total time since the application started and the time elapsed 
	 * between the last two frames (delta time).
	 */
	class TimeMetrics : public spk::Singleton<TimeMetrics>
	{
		friend class spk::Singleton<TimeMetrics>;
		friend class TimeModule;

	private:
		/**
		 * @brief The total time since the application started.
		 * 
		 * This variable is used to store the total time since the application started. 
		 * It is updated every frame.
		 */
		long long _time;
		long long _timeMicro;

		/**
		 * @brief The time elapsed between the last two frames.
		 * 
		 * This variable is used to store the time elapsed between the last two frames, 
		 * also known as the delta time. It is updated every frame.
		 */
		long long _deltaTime;

		/**
		 * @brief Updates the time metrics.
		 * 
		 * This function is used to update the time and delta time variables. 
		 * It is called every frame by the TimeModule class.
		 */
		void _updateMetrics();

		/**
		 * @brief Construct a new Time Metrics object.
		 * 
		 * This is the default constructor for the TimeMetrics class. 
		 * It initializes the time and delta time to zero.
		 */
		TimeMetrics();

	public:
		/**
		 * @brief Get the total time since the application started.
		 * 
		 * This function is used to get the total time since the application started.
		 * 
		 * @return The total time since the application started in milliseconds.
		 */
		constexpr const long long& time() const { return (_time); }

		constexpr const long long& timeMicro() const { return (_timeMicro); }

		/**
		 * @brief Get the time elapsed between the last two frames.
		 * 
		 * This function is used to get the time elapsed between the last two frames, also known as the delta time.
		 * 
		 * @return The delta time in milliseconds.
		 */
		constexpr const long long& deltaTime() const { return (_deltaTime); }

		/**
		 * \brief Function to put the current thread to sleep for at least the specified duration.
		 * \param p_millisecond The minimum number of milliseconds the thread should sleep.
		 */
		void sleepAtLeast(size_t p_millisecond);
	};
}
