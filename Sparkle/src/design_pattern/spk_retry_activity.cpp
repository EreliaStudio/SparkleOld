#include "design_pattern/spk_activity.hpp"

namespace spk
{
	void RetryActivity::_onEnter()
	{
		_moveNextStepAndExecute();
	}

	void RetryActivity::_execute()
	{
		if (_funct() == true)
			_moveNextStepAndExecute();
	}

	void RetryActivity::_onExit()
	{
		_moveNextStepAndExecute();
	}

	RetryActivity::RetryActivity() :
		_funct(nullptr)
	{

	}

	RetryActivity::RetryActivity(const std::function<bool()> &p_funct) :
		_funct(p_funct)
	{

	}

	void RetryActivity::setFunct(const std::function<bool()> &p_funct)
	{
		_funct = p_funct;
	}
}