#pragma once

#include "system/spk_time_metrics.hpp"
#include "threading/spk_persistent_worker.hpp"

namespace spk
{
	class Chronometer
	{
	private:
		long long _start;
		long long _duration;

	public:
		Chronometer();
		~Chronometer();

		void start();
		const long long& duration();
		const long long& stop();
	};
}