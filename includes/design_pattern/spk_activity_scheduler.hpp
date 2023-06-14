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
		ActivityScheduler();

		void addActivity(Activity* p_activity);

		void execute();
	};
}