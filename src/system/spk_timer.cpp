#include "system/spk_timer.hpp"
#include <stdexcept>

namespace spk
{
	Timer::Timer(const long long& p_duration) :
		_duration(p_duration),
		_status(State::Idle),
		_startTime(0),
		_endTime(0)
	{
		if (spk::TimeMetrics::instance() == nullptr)
		{
			throw std::runtime_error("Can't create a Timer without an spk::Application");
		}
	}

	const long long& Timer::duration() const
	{
		return (_duration);
	}

	void Timer::setDuration(const long long& p_duration)
	{
		if (_status == State::Running)
			throw std::runtime_error("Timer: duration edited while running");

		_duration = p_duration;
	}

	long long Timer::remainingDuration() const
	{
		if (_status == State::Idle)
			return 0;

		return (_endTime - TimeMetrics::instance()->time());
	}

	void Timer::start()
	{
		if (_status == State::Running)
			return;

		_status = State::Running;
		_startTime = TimeMetrics::instance()->time();
		_endTime = _startTime + _duration;
	}

	void Timer::stop()
	{
		if (_status == State::Idle)
			return;

		_status = State::Idle;
	}

	void Timer::_updateStatus() const
	{
		if (TimeMetrics::instance()->time() > _endTime)
			_status = State::Timeout;
		else
			_status = State::Running;
	}

	const Timer::State& Timer::status() const
	{
		if (_status == State::Running)
			_updateStatus();
		return _status;
	}

	bool Timer::isRunning() const
	{
		return status() == State::Running;
	}
}