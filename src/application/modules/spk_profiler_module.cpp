#include "application/modules/spk_profiler_module.hpp"
#include "debug/spk_profiler.hpp"

namespace spk
{
	ProfilerModule::ProfilerModule() :
		_counterTimer(250)
	{
		spk::Profiler::instanciate();
	}

	ProfilerModule::~ProfilerModule()
	{
		spk::Profiler::release();
	}

	void ProfilerModule::updateFPS()
	{
		spk::Profiler::instance()->_fpsCounter++;
	}
	
	void ProfilerModule::updateUPS()
	{
		spk::Profiler::instance()->_upsCounter++;
	}

	void ProfilerModule::updateCounters()
	{
		if (_counterTimer.isRunning() == false)
		{
			spk::Profiler::instance()->_normalizedFPS = spk::Profiler::instance()->_fpsCounter * 4;
			spk::Profiler::instance()->_normalizedUPS = spk::Profiler::instance()->_upsCounter * 4;
			
			spk::Profiler::instance()->_fpsCounter = 0;
			spk::Profiler::instance()->_upsCounter = 0;
			_counterTimer.start();
		}
	}
}