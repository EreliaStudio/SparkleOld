#pragma once

#include "widget/spk_widget_interface.hpp"
#include "design_pattern/spk_activity.hpp"

namespace spk::Widget
{
	class Activity;

	class ActivityScheduler : public spk::Widget::NoGraphics
	{
	private:
		bool _onUpdate();
	
	public:
		ActivityScheduler(const std::wstring& p_name);
		~ActivityScheduler();
	};
	
	void launchActivity(spk::Activity* p_activity);
}