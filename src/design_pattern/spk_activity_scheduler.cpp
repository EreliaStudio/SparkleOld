#include "design_pattern/spk_activity_scheduler.hpp"

namespace spk
{
	ActivityScheduler::ActivityScheduler()
	{

	}

	void ActivityScheduler::addActivity(Activity* p_activity)
	{
		p_activity->_start();
		_activities.push_back(p_activity);
	}

	void ActivityScheduler::execute()
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
}