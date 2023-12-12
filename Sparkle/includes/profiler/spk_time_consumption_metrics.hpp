#pragma once

#include "profiler/spk_imetrics.hpp"
#include "system/spk_chronometer.hpp"

namespace spk
{
	class TimeConsumption : public IMetrics<long long>
	{
	private:
		spk::Chronometer _chrono;
		long long _totalDuration;

	public:
		TimeConsumption();

		spk::JSON::Object emitReport() const;

		void start();
		void stop();

		double CPUUsage() const;
		const long long& totalDuration() const;
	};
}