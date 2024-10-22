#include "profiler/spk_time_consumption_metrics.hpp"

namespace spk
{
	TimeConsumption::TimeConsumption() :
		_totalDuration(0)
	{

	}

	bool TimeConsumption::needEmition() const
	{
		if (cardinal() == 0)
			return (false);
		return (true);
	}

	spk::JSON::Object TimeConsumption::emitReport() const
	{
		spk::JSON::Object result;

		result.addAttribute(L"Min").set<long>(static_cast<long>(min()));
		result.addAttribute(L"Max").set<long>(static_cast<long>(max()));
		result.addAttribute(L"Average").set<long>(static_cast<long>(average()));
		result.addAttribute(L"Total duration").set<long>(static_cast<long>(spk::TimeMetrics::nanosecondToMillisecond(totalDuration())));
		result.addAttribute(L"CPU Usage").set<double>((totalDuration() * 100.0) / static_cast<double>(spk::TimeMetrics::millisecondToNanosecond(spk::TimeMetrics::instance()->programDuration())));
		result.addAttribute(L"Cardinal").set<long>(static_cast<long>(cardinal()));

		return (result);
	}

	void TimeConsumption::start()
	{
		_chrono.start();
	}

	void TimeConsumption::stop()
	{
		_chrono.stop();
		_totalDuration += _chrono.duration();
		_insertValue(_chrono.duration());
	}
	
	const long long& TimeConsumption::totalDuration() const
	{
		return (_totalDuration);
	}
}