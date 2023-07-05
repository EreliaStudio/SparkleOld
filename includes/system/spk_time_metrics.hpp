#pragma once

#include "design_pattern/spk_singleton.hpp"
#include "application/modules/spk_time_module.hpp"


namespace spk
{
    /**
     * \class TimeMetrics
     * \brief A singleton class to manage time-related metrics for the software.
     *
     * This class is responsible for keeping track of the software's running time
     * and other time-related metrics. 
	 * This ensures consistency across the entire software.
     */
	class TimeMetrics : public spk::Singleton<TimeMetrics>
	{
		friend class spk::Singleton<TimeMetrics>;
		friend class TimeModule;

	private:
		/**
		 * \brief A long long variable representing the total running time of the software.
		 */
		long long _time;

		/**
		 * \brief A long long variable representing the delta time or time elapsed since the last frame update.
		 */
		long long _deltaTime;

		/**
		 * \brief A private function to update the time metrics.
		 */
		void _updateMetrics();

		/**
		 * \brief Private constructor to implement the Singleton pattern.
		 */
		TimeMetrics();

	public:

		/**
		 * \brief Accessor function to get the total running time of the software.
		 * \return A const reference to the total running time of the software variable.
		 */
		constexpr const long long& time() const { return (_time); }

		/**
		 * \brief Accessor function to get the time elapsed since the last frame update.
		 * \return A const reference to the the time elapsed since the last frame update.
		 */
		constexpr const long long& deltaTime() const { return (_deltaTime); }

		/**
		 * \brief Function to put the current thread to sleep for at least the specified duration.
		 * \param p_millisecond The minimum number of milliseconds the thread should sleep.
		 */
		void sleepAtLeast(size_t p_millisecond);
	};
}
