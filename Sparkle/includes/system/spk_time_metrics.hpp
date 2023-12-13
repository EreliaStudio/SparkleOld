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

		/**
		 * @brief The time elapsed between the last two frames.
		 * 
		 * This variable is used to store the time elapsed between the last two frames, 
		 * also known as the delta time. It is updated every frame.
		 */
		long long _deltaTime;

		/**
		 * @brief The time mark of the start of the program.
		 */
		long long _programStartingTime;

		/**
		 * @brief The time elapsed since start of the program.
		 */
		long long _programDuration;

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
		 * @brief Static method to get the current system time
		 * 
		 * @return The time as epoch value
		*/
		static long long currentTime();

		/**
		 * @brief Converts time from nanoseconds to milliseconds.
		 *
		 * This function takes a time value in nanoseconds and converts it to milliseconds.
		 * As there are 1,000,000 nanoseconds in a millisecond, the function divides the input value by 1,000,000.
		 *
		 * @param p_nanosecond The time in nanoseconds to be converted.
		 * @return The time in milliseconds.
		 */
		static long long nanosecondToMillisecond(const long long& p_nanosecond);

		/**
		 * @brief Converts time from milliseconds to nanoseconds.
		 *
		 * This function takes a time value in milliseconds and converts it to nanoseconds.
		 * As there are 1,000,000 nanoseconds in a millisecond, the function multiplies the input value by 1,000,000.
		 *
		 * @param p_millisecond The time in milliseconds to be converted.
		 * @return The time in nanoseconds.
		 */
		static long long millisecondToNanosecond(const long long& p_millisecond);

		/**
		 * @brief Get the total time since the application started.
		 * 
		 * This function is used to get the total time since the application started.
		 * 
		 * @return The total time since the application started in milliseconds.
		 */
		constexpr const long long& time() const { return (_time); }

		/**
		 * @brief Get the time elapsed between the last two frames.
		 * 
		 * This function is used to get the time elapsed between the last two frames, also known as the delta time.
		 * 
		 * @return The delta time in milliseconds.
		 */
		constexpr const long long& deltaTime() const { return (_deltaTime); }

		/**
		 * @brief Get the time elapsed since the start of the program.
		 * 
		 * @return The delta time since start and now.
		 */
		constexpr const long long& programDuration() const { return (_programDuration); }

		/**
		 * \brief Function to put the current thread to sleep for at least the specified duration.
		 * \param p_millisecond The minimum number of milliseconds the thread should sleep.
		 */
		static void sleepAtLeast(long long p_millisecond);
	};
}
