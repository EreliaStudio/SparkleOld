#pragma once

#include "widget/spk_abstract_widget.hpp"

namespace spk
{
	class Activity;

	class ActivitySchedulerWidget : public spk::NoGraphicsWidget
	{
	private:
		bool _onUpdate();
	
	public:
		ActivitySchedulerWidget(const std::wstring& p_name);
		~ActivitySchedulerWidget();
	};
	
	void launchActivity(spk::Activity* p_activity);
}