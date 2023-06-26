#pragma once

#include "system/spk_time_metrics.hpp"
#include "threading/spk_persistent_worker.hpp"

namespace spk
{
	class Chronometer
	{
	private:
		long long _start;
		TimeModule _timeModule;

	public:
		Chronometer();
		~Chronometer();

		void start();
		long long stop();
	};
}