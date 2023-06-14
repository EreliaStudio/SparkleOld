#include "design_pattern/spk_activity.hpp"

namespace spk
{
	void Activity::_update()
	{
		switch (_internalWorkingState)
		{
		case Status::OnEnter:
			_onEnter();
			break;
		case Status::Running:
			_execute();
			break;
		case Status::OnExit:
			_onExit();
			break;
		default:
			break;
		}
	}

	void Activity::_moveNextStep()
	{
		switch (_internalWorkingState)
		{
		case Status::OnEnter:
			_internalWorkingState = Status::Running;
			break;
		case Status::Running:
			_internalWorkingState = Status::OnExit;
			break;
		case Status::OnExit:
			_internalWorkingState = Status::Complete;
			break;
		default:
			break;
		}
	}

	void Activity::_moveNextStepAndExecute()
	{
		_moveNextStep();
		_update();
	}

	void Activity::_start()
	{
		_internalWorkingState = Status::OnEnter;
	}

	Activity::Activity()
	{
		_internalWorkingState = Status::Stopped;
	}

	const Activity::Status& Activity::state() const
	{
		return (_internalWorkingState);
	}

	bool Activity::isCompleted() const
	{
		return (_internalWorkingState == Status::Complete);
	}
}

