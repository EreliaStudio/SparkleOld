#include "system/spk_chronometer.hpp"
#include <stdexcept>
#include "iostream/spk_iostream.hpp"

const int UNINITIALIZED = -1;

namespace spk
{
	Chronometer::Chronometer():
		_start(Clock::now()),
        _duration(Duration::zero()),
        _totalDuration(Duration::zero()),
		_isRunning(false)
	{
		if (spk::TimeMetrics::instance() == nullptr)
		{
			throw std::runtime_error("Can't create a Chronometer without an spk::Application");
		}
	}

	Chronometer::~Chronometer()
	{
	}
	
	void Chronometer::reset()
	{
		if (hasBeenStarted() == false)
			throw std::runtime_error("Can't reset a never used chronometer");

		_start = Clock::now();
        _duration = Duration::zero();
        _totalDuration = Duration::zero();
		_isRunning = false;
	}

	void Chronometer::start()
	{
		if (isRunning() == true)
			throw std::runtime_error("Can't start an already started chronometer");

		_start = Clock::now();
        _duration = Duration::zero();
        _totalDuration = Duration::zero();
		_isRunning = true;
	}

	void Chronometer::resume()
	{
		if (isRunning() == true)
			throw std::runtime_error("Can't resume an already started chronometer");
		if (hasBeenStarted() == false)
			throw std::runtime_error("Can't resume a chronometer that haven't been launched previously");

		_start = Clock::now() - _totalDuration;
		_isRunning = true;
	}

	bool Chronometer::hasBeenStarted() const
	{
		return (_start.time_since_epoch() != Duration::zero());
	}

	bool Chronometer::isRunning() const
	{
		return (_isRunning);
	}

	long long Chronometer::duration() const
	{
		if (isRunning())
        {
            _duration = std::chrono::duration_cast<Duration>(Clock::now() - _start);
        }
        return ((_duration + _totalDuration).count());
	}

	long long Chronometer::stop()
	{
		if (isRunning() == false)
			throw std::runtime_error("Can't stop an already stopped chronometer");

		_totalDuration += std::chrono::duration_cast<Duration>(Clock::now() - _start);
        _duration = Duration::zero();
        _isRunning = false;
        return (_totalDuration.count());
	}
}
