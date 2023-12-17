#pragma once

#include "widget/spk_widget.hpp"
#include "design_pattern/spk_activity.hpp"

namespace spk
{
	class Activity;

	/**
	 * @class ActivitySchedulerManager
	 * @brief Class for managing scheduling of activities.
	 *
	 * This class is a no-graphics widget that manages the scheduling and execution of activities. 
	 * Activities can be launched, which means they are added to the schedule and executed when appropriate.
	 */
	class ActivitySchedulerManager : public spk::NoGraphicsWidget
	{
	private:
		/**
		 * @brief The onUpdate function that is called when the scheduler updates.
		 *
		 * This function is called when the scheduler updates, and is responsible for managing the execution 
		 * of scheduled activities.
		 *
		 * @return Returns true if the update was successful, false otherwise.
		 */
		bool _onUpdate();
	
	public:
		/**
		 * @brief Constructs an ActivitySchedulerManager with a name.
		 *
		 * @param p_name The name of the scheduler.
		 */
		ActivitySchedulerManager(const std::wstring& p_name);

		/**
		 * @brief Destroys the ActivitySchedulerManager.
		 *
		 * This destructor ensures that all scheduled activities are properly cleaned up when the scheduler is destroyed.
		 */
		~ActivitySchedulerManager();
	};

	/**
	 * @brief Launches an activity.
	 *
	 * This function schedules an activity to be executed. The activity will be run when the scheduler
	 * updates and determines that it is time for the activity to execute.
	 *
	 * @param p_activity The activity to be launched.
	 */
	void launchActivity(spk::Activity* p_activity);
}
