#pragma once

#include <vector>
#include "design_pattern/spk_activity.hpp"

namespace spk
{
	class ActivityScheduler
	{
	private:
		std::vector<Activity*> _activities;

	public:
		ActivityScheduler()
		{

		}

		void addActivity(Activity* p_activity)
		{
            p_activity->_start();
			_activities.push_back(p_activity);
		}

		void execute()
		{
			for (size_t i = 0; i < _activities.size();)
			{
				_activities[i]->_update();

				if (_activities[i]->isCompleted() == true)
				{
					_activities.erase(_activities.begin() + i);
				}
				else
				{
					i++;
				}
			}
		}
	};
}