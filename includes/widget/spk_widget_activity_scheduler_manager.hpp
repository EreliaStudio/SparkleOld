#pragma once

#include "widget/spk_widget_interface.hpp"
#include "design_pattern/spk_activity.hpp"

namespace spk::Widget
{
	class Activity;

	class ActivitySchedulerManager : public spk::Widget::NoGraphics
	{
	private:
		bool _onUpdate();
	
	public:
		ActivitySchedulerManager(const std::wstring& p_name);
		~ActivitySchedulerManager();
	};
	
	void launchActivity(spk::Activity* p_activity);
}