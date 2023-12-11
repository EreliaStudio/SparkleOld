#include "design_pattern/spk_activity.hpp"

namespace spk
{
	void TimedActivity::_onEnter()
	{
		_timer.start();
	}

	void TimedActivity::_execute()
	{
		if (_timer.isRunning() == false)
			_moveNextStepAndExecute();
	}
	
	void TimedActivity::_onExit()
	{
		_funct();
		_timer.stop();
		_moveNextStepAndExecute();
	}

	TimedActivity::TimedActivity() :
		_funct(nullptr),
		_timer(0)
	{

	}

	TimedActivity::TimedActivity(const size_t &p_delay, const std::function<void()> &p_funct) :
		_funct(p_funct),
		_timer(p_delay)
	{

	}

	void TimedActivity::setFunct(const std::function<void()> &p_funct)
	{
		_funct = p_funct;
	}

	void TimedActivity::setDelay(const size_t &p_delay)
	{
		_timer.setDuration(p_delay);
	}
}