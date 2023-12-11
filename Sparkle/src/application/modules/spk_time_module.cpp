#include "application/modules/spk_time_module.hpp"
#include "system/spk_time_metrics.hpp"
#include <chrono>
#include "iostream/spk_iostream.hpp"

namespace spk
{
	TimeModule::TimeModule()
	{
		setMaxFPS(60);
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
	
	void TimeModule::setMaxFPS(const size_t& p_nbFPSPerSecond)
	{
		_msPerFrame = 1000 / p_nbFPSPerSecond;
	}

	void TimeModule::wait()
	{
		static thread_local long long nextFrame = spk::TimeMetrics::currentTime() + _msPerFrame;

		long long currentTime = spk::TimeMetrics::currentTime();

		if (nextFrame > currentTime)
			spk::TimeMetrics::sleepAtLeast(nextFrame - currentTime);
		nextFrame += _msPerFrame;
	}
}