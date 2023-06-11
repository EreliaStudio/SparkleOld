#pragma once

#include <chrono>
#include "design_pattern/spk_singleton.hpp"
#include "application/modules/spk_time_module.hpp"

namespace spk
{
	class TimeMetrics : public spk::Singleton<TimeMetrics>
	{
		friend class spk::Singleton<TimeMetrics>;
		friend class TimeModule;
	private:
		long long _time;
		long long _deltaTime;

		void _updateMetrics();
		TimeMetrics();

	public:

		constexpr const long long& time() const { return (_time); }
		constexpr const long long& deltaTime() const { return (_deltaTime); }
	};
}