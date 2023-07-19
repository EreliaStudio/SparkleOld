#include "widget/spk_activity_scheduler_widget.hpp"
#include "design_pattern/spk_activity_scheduler.hpp"

namespace spk::Widget
{	
	bool ActivityScheduler::_onUpdate()
	{
		spk::Singleton<spk::ActivityScheduler>::instance()->execute();
		return (false);
	}
	
	ActivityScheduler::ActivityScheduler(const std::wstring& p_name) : 
		spk::Widget::NoGraphics(p_name)
	{
		spk::Singleton<spk::ActivityScheduler>::instanciate();
	}
	
	ActivityScheduler::~ActivityScheduler()
	{
		spk::Singleton<spk::ActivityScheduler>::release();
	}

	void launchActivity(spk::Activity* p_activity)
	{
		spk::Singleton<spk::ActivityScheduler>::instance()->addActivity(p_activity);
	}
}