#pragma once

#include "widget/spk_abstract_widget.hpp"

namespace spk
{
	class Activity;

	class ActivitySchedulerManager : public spk::NoGraphicsWidget
	{
	private:
		bool _onUpdate();
	
	public:
		ActivitySchedulerManager(const std::wstring& p_name);
		~ActivitySchedulerManager();
	};
	
	launchActivity(spk::Activity* p_activity);
}