#include "system/spk_chronometer.hpp"

namespace spk
{
	Chronometer::Chronometer():
		_start(0)
	{
	}

	Chronometer::~Chronometer()
	{
	}

	void Chronometer::start()
	{
		_start = spk::TimeMetrics::instance()->time();
	}

	long long Chronometer::stop()
	{
		if (_start == 0)
			return (0);
		return (spk::TimeMetrics::instance()->time() - _start);
	}
}