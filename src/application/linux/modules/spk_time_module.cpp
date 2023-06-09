#include "application/linux/modules/spk_time_module.hpp"

namespace spk
{
	TimeModule::TimeModule()
	{
		spk::Singleton<spk::TimeMetrics>::instanciate();
	}

	TimeModule::~TimeModule()
	{
		spk::Singleton<spk::TimeMetrics>::release();
	}

	void TimeModule::update()
	{
		spk::Singleton<spk::TimeMetrics>::instance()->_updateMetrics();
	}
}