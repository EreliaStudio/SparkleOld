#include "application/modules/spk_profiler_module.hpp"

namespace spk
{
	ProfilerModule::ProfilerModule()
	{
		
	}

	ProfilerModule::~ProfilerModule()
	{
		//emitReport();
	}

	void ProfilerModule::emitReport()
	{
		spk::JSON::Object report = _profiler.emitReport();

		spk::cout << "Report : " << std::endl;
		spk::cout << report << std::endl;
	}
}