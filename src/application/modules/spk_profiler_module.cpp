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

	}
	
	void ProfilerModule::updateUPS()
	{

	}

	void ProfilerModule::updateCounters()
	{
		
	}
}