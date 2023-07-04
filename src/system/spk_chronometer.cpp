#include "system/spk_chronometer.hpp"

namespace spk
{
	Chronometer::Chronometer():
		_start(0),
		_duration(0),
		_isRunning(false)
	{
	}

	Chronometer::~Chronometer()
	{
	}

	void Chronometer::start()
	{
		_start = spk::TimeMetrics::instance()->time();
		_duration = 0;
		_isRunning = true;
	}
	
	void Chronometer::pause()
	{
		_duration += spk::TimeMetrics::instance()->time() - _start;
		_start = 0;
		_isRunning = false;

	}

	void Chronometer::resume()
	{
		_start = spk::TimeMetrics::instance()->time();
		_isRunning = true;
	}

	bool Chronometer::isRunning()
	{
		return (_isRunning);
	}

	const long long& Chronometer::duration() const
	{
		if (_start != 0)
			_duration += spk::TimeMetrics::instance()->time() - _start;
		return (_duration);
	}

	const long long& Chronometer::stop()
	{
		if (_start != 0)
		{
			_duration += spk::TimeMetrics::instance()->time() - _start;
			_start = 0;
			_isRunning = false;

		}
		return (_duration);
	}
}