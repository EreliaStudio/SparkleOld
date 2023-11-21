#pragma once

#include <string>
#include <map>
#include <stdexcept>

#include "system/spk_chronometer.hpp"
#include "design_pattern/spk_singleton.hpp"

namespace spk
{
	class ProfilerModule;

	class Profiler : public spk::Singleton<Profiler>
	{
		friend class spk::Singleton<Profiler>;
		friend class ProfilerModule;

	private:
		
	public:
		
	};
}
