#include "system/spk_time_metrics.hpp"
#include <chrono>
#include <thread>

namespace spk
{
	long long TimeMetrics::currentTime()
	{
		return (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	}

	void TimeMetrics::_updateMetrics()
	{
		long long now = currentTime();

		_deltaTime = now - _time;
		_time = now;
	}

	TimeMetrics::TimeMetrics():
		_time(0),
		_deltaTime(0)
	{
		std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();

		_time = currentTime();

		_updateMetrics();
	}
	
	void TimeMetrics::sleepAtLeast(long long p_millisecond)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(p_millisecond));
	}

}