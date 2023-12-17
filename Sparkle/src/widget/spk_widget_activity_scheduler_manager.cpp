#include "widget/spk_widget_activity_scheduler_manager.hpp"
#include "design_pattern/spk_activity_scheduler.hpp"

namespace spk
{	
	bool ActivitySchedulerManager::_onUpdate()
	{
		spk::Singleton<spk::ActivityScheduler>::instance()->execute();
		return (false);
	}
	
	ActivitySchedulerManager::ActivitySchedulerManager(const std::wstring& p_name) : 
		spk::NoGraphicsWidget(p_name)
	{
		spk::Singleton<spk::ActivityScheduler>::instanciate();
	}
	
	ActivitySchedulerManager::~ActivitySchedulerManager()
	{
		spk::Singleton<spk::ActivityScheduler>::release();
	}

	void launchActivity(spk::Activity* p_activity)
	{
		spk::Singleton<spk::ActivityScheduler>::instance()->addActivity(p_activity);
	}
}