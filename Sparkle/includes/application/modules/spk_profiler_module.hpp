#pragma once

#include "profiler/spk_profiler.hpp"

namespace spk
{
	class ProfilerModule
	{
	private:
		spk::Profiler _profiler;

	public:
		ProfilerModule();

		~ProfilerModule();

		void emitReport();

		spk::Profiler& profiler() {return (_profiler);}
		const spk::Profiler& profiler() const {return (_profiler);}
	};
}
