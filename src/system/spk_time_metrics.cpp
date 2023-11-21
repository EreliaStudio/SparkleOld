#include "system/spk_time_metrics.hpp"
#include <chrono>
#include <thread>

namespace spk
{
	void TimeMetrics::_updateMetrics()
	{
		long long nowMicro = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		long long now = nowMicro / 1000;

		_deltaTime = now - _time;
		_time = now;
		_timeMicro = nowMicro;
	}

	TimeMetrics::TimeMetrics():
		_time(0),
		_timeMicro(0),
		_deltaTime(0)
	{
		std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();

		_timeMicro = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		_time = _timeMicro / 1000;

		_updateMetrics();
	}
	
	void TimeMetrics::sleepAtLeast(size_t p_millisecond)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(p_millisecond));
	}

}