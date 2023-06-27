#include "system/spk_chronometer.hpp"

namespace spk
{
	Chronometer::Chronometer():
		_start(0),
		_duration(0)
	{
	}

	Chronometer::~Chronometer()
	{
	}

	void Chronometer::start()
	{
		_start = spk::TimeMetrics::instance()->time();
		_duration = 0;
	}

	const long long& Chronometer::duration()
	{
		if (_start == 0 || _duration != 0)
			return (_duration);
		_duration = spk::TimeMetrics::instance()->time() - _start;
		return (_duration);
	}

	const long long& Chronometer::stop()
	{
		if (_start == 0)
			return (_duration);
		_duration = spk::TimeMetrics::instance()->time() - _start;
		return (_duration);
	}
}