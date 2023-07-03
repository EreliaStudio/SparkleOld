#pragma once

#include <vector>
#include "design_pattern/spk_activity.hpp"

namespace spk
{
	/**
	 * @brief Allows activitee to follow each other.
	 */
	class ActivityScheduler
	{
	private:
		std::vector<Activity*> _activities;

	public:
		/**
		 * @brief Construct a new Activity Scheduler object.
		 */
		ActivityScheduler();
		
		/**
		 * @brief Add an activity at the end of the schedule.
		 * @param p_activity The activity to add.
		 */
		void addActivity(Activity* p_activity);

		/**
		 * @brief Start activities one after each other until each one are complete.
		 */
		void execute();
	};
}