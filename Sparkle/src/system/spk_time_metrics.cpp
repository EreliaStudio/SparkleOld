#include "system/spk_time_metrics.hpp"
#include <chrono>
#include <thread>

namespace spk
{
	long long TimeMetrics::currentTime()
	{
		return (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	}

	long long TimeMetrics::nanosecondToMillisecond(const long long& p_nanosecond)
	{
		return (p_nanosecond / 1000000);
	}
	
	long long TimeMetrics::millisecondToNanosecond(const long long& p_millisecond)
	{
		return (p_millisecond * 1000000);
	}

	void TimeMetrics::_updateMetrics()
	{
		long long now = currentTime();

		_deltaTime = now - _time;
		_time = now;
		_programDuration = now - _programStartingTime;
	}

	TimeMetrics::TimeMetrics():
		_time(0),
		_deltaTime(0)
	{
		_time = _programStartingTime = currentTime();

		_updateMetrics();
	}
	
	void TimeMetrics::sleepAtLeast(long long p_millisecond)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(p_millisecond));
	}

}