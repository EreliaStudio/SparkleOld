#include "application/modules/spk_profiler_module.hpp"
#include "debug/spk_profiler.hpp"

namespace spk
{
	ProfilerModule::ProfilerModule() :
		_counterTimer(100),
		_counterRatio(1000.0f / static_cast<float>(_counterTimer.duration()))
	{
		spk::Profiler::instanciate();
		_counterTimer.start();
	}

	ProfilerModule::~ProfilerModule()
	{
		spk::Profiler::release();
	}

	void ProfilerModule::updateFPS()
	{
		spk::Profiler::instance()->fpsCounter().trigger();
	}
	
	void ProfilerModule::updateUPS()
	{
		spk::Profiler::instance()->upsCounter().trigger();
	}

	void ProfilerModule::updateCounters()
	{
		if (_counterTimer.isRunning() == false)
		{
			spk::Profiler::instance()->fpsCounter().set(static_cast<size_t>(spk::Profiler::instance()->fpsCounter().value() * _counterRatio));
			spk::Profiler::instance()->upsCounter().set(static_cast<size_t>(spk::Profiler::instance()->upsCounter().value() * _counterRatio));

			spk::Profiler::instance()->fpsCounter().reset();
			spk::Profiler::instance()->upsCounter().reset();
			_counterTimer.start();
		}
	}
}