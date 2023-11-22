#include "application/modules/spk_time_module.hpp"
#include "system/spk_time_metrics.hpp"
#include <chrono>
#include "iostream/spk_iostream.hpp"
#include "debug/spk_profiler.hpp"

namespace spk
{
	TimeModule::TimeModule()
	{
		spk::Singleton<spk::TimeMetrics>::instanciate();
		updateTimeMetrics();
	}

	TimeModule::~TimeModule()
	{
		spk::Singleton<spk::TimeMetrics>::release();
	}

	void TimeModule::updateTimeMetrics()
	{
		spk::Singleton<spk::TimeMetrics>::instance()->_updateMetrics();
	}
	
	void TimeModule::wait()
	{
		static thread_local long long nextFrame = spk::TimeMetrics::currentTime() + 16;

		long long currentTime = spk::TimeMetrics::currentTime();

		if (nextFrame > currentTime)
			spk::TimeMetrics::sleepAtLeast(nextFrame - currentTime);
		nextFrame += 16;
	}
}