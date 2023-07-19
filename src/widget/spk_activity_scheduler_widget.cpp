#include "widget/spk_activity_scheduler_widget.hpp"
#include "design_pattern/spk_activity_scheduler.hpp"

namespace spk
{	
	bool ActivitySchedulerWidget::_onUpdate()
	{
		spk::Singleton<spk::ActivityScheduler>::instance()->execute();
		return (false);
	}
	
	ActivitySchedulerWidget::ActivitySchedulerWidget(const std::wstring& p_name) : 
		spk::NoGraphicsWidget(p_name)
	{
		spk::Singleton<spk::ActivityScheduler>::instanciate();
	}
	
	ActivitySchedulerWidget::~ActivitySchedulerWidget()
	{
		spk::Singleton<spk::ActivityScheduler>::release();
	}

	void launchActivity(spk::Activity* p_activity)
	{
		spk::Singleton<spk::ActivityScheduler>::instance()->addActivity(p_activity);
	}
}