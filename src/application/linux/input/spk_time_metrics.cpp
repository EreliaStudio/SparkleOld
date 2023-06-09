#include "application/linux/input/spk_time_metrics.hpp"

namespace spk
{
	void TimeMetrics::_updateMetrics()
	{
		long long now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		_deltaTime = now - _time;
		_time = now;
	}

	TimeMetrics::TimeMetrics():
		_time(0),
		_deltaTime(0)
	{
		std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();

		_time = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

		_updateMetrics();
	}
}