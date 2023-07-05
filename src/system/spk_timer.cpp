#include "system/spk_timer.hpp"

namespace spk
{
	Timer::Timer() :
		_duration(0),
		_status(State::Idle),
		_startTime(0)
	{
		if (spk::TimeMetrics::instance() == nullptr)
		{
			throw std::runtime_error("Can't create a Timer without an spk::Application");
		}
	}

	void Timer::setDuration(const long long& p_duration)
	{
		if (_status == State::Running)
			throw std::runtime_error("Timer: duration edited while running");

		_duration = p_duration;
	}

	const long long& Timer::totalDuration() const
	{
		return _duration;
	}

	long long Timer::remainingDuration() const
	{
		if (_status == State::Idle)
			return 0;

		long long result = _duration - (TimeMetrics::instance()->time() - _startTime);

		if (result <= 0 && _status == State::Running)
		{
			_status = State::Timeout;
		}
		return (result);
	}

	void Timer::start()
	{
		if (_status == State::Running)
			return;

		_status = State::Running;
		_startTime = TimeMetrics::instance()->time();
	}

	void Timer::stop()
	{
		if (_status == State::Idle)
			return;

		_status = State::Idle;
	}

	const Timer::State& Timer::status() const
	{
		if (_status == State::Running)
			remainingDuration();
		return _status;
	}

	bool Timer::isRunning() const
	{
		return status() == State::Running;
	}
}