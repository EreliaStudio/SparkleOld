#include "system/spk_chronometer.hpp"

namespace spk
{
	Chronometer::Chronometer():
		_start(0),
		_duration(0),
		_totalDuration(0),
		_isRunning(false)
	{
	}

	Chronometer::~Chronometer()
	{
	}

	void Chronometer::start()
	{
		if (isRunning() == true)
			throw std::runtime_error("Can't start an already started chronometer");

		_start = spk::TimeMetrics::instance()->time();
		_duration = 0;
		_totalDuration = 0;
		_isRunning = true;
	}

	void Chronometer::resume()
	{
		if (isRunning() == true)
			throw std::runtime_error("Can't resume an already started chronometer");

		_start = spk::TimeMetrics::instance()->time();
		_isRunning = true;
	}

	bool Chronometer::isRunning() const
	{
		return (_isRunning);
	}

	const long long Chronometer::duration() const
	{
		if (isRunning() == true)
			_duration = spk::TimeMetrics::instance()->time() - _start;
		return (_duration + _totalDuration);
	}

	const long long& Chronometer::stop()
	{
		if (isRunning() == false)
			throw std::runtime_error("Can't stop an already stopped chronometer");

		_totalDuration = duration();
		_start = 0;
		_isRunning = false;
		return (_totalDuration);
	}
}