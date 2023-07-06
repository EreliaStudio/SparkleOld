#include "application/modules/spk_profiler_module.hpp"
#include "debug/spk_profiler.hpp"

namespace spk
{
	ProfilerModule::ProfilerModule()
	{
		spk::Profiler::instanciate();

		_oneSecondTime.setDuration(250);
		_resetCounter();
	}

	ProfilerModule::~ProfilerModule()
	{
		spk::Profiler::release();
	}
	
	void ProfilerModule::_resetCounter()
	{
		_renderIPSCounter = 0;
		_updateIPSCounter = 0;
	}

	void ProfilerModule::update()
	{
		if (_oneSecondTime.isRunning() == false)
		{
			spk::Profiler::instance()->setCounter(Profiler::RENDER_IPS_COUNTER, _renderIPSCounter * 4);
			spk::Profiler::instance()->setCounter(Profiler::UPDATE_IPS_COUNTER, _updateIPSCounter * 4);

			_resetCounter();

			_oneSecondTime.start();
		}
	}

	void ProfilerModule::increaseRenderIPS()
	{
		_renderIPSCounter++;
	}
	
	void ProfilerModule::increaseUpdateIPS()
	{
		_updateIPSCounter++;
	}
}