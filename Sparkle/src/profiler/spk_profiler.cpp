#include "profiler/spk_profiler.hpp"

namespace spk
{
	Profiler::Profiler()
	{

	}

	Profiler::~Profiler()
	{
		for (const auto& metrics : _metrics)
			delete (metrics.second);
	}

	spk::JSON::Object Profiler::emitReport() const
	{
		spk::JSON::Object result;

		for (const auto& metrics : _metrics)
		{
			result.addAttribute(metrics.first) = metrics.second->emitReport();
		}

		return (result);
	}
}