#include "application/modules/spk_time_module.hpp"
#include "system/spk_time_metrics.hpp"

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