#include "system/spk_chronometer.hpp"

namespace spk
{
	Chronometer::Chronometer():
		_start(0),
		_timeModule()
	{
	}

	Chronometer::~Chronometer()
	{
	}

	void Chronometer::start()
	{
		_timeModule.update();
		_start = spk::TimeMetrics::instance()->time();
	}

	long long Chronometer::stop()
	{
		if (_start == 0)
			return (0);
		_timeModule.update();
		return (spk::TimeMetrics::instance()->time() - _start);
	}
}