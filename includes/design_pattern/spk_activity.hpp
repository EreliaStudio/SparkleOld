#pragma once

#include <functional>

namespace spk
{
	class Activity
	{
        friend class ActivityManager;

	public:
		enum class Status
		{
			OnEnter,
			Running,
			OnExit,
			Complete
		};
		Status _internalWorkingState;

		void _update()
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
			}
		}

		void _moveNextStep()
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
			}
		}

		void _moveNextStepAndExecute()
		{
			_moveNextStep();
			_update();
		}

	protected:
		virtual void _onEnter() = 0;
		virtual void _execute() = 0;
		virtual void _onExit() = 0;


	public:
		Activity()
		{
			_internalWorkingState = Status::Complete;
		}

        const Status& state() const
        {
            return (_internalWorkingState);
        }

		bool isCompleted() const
        {
            return (_internalWorkingState == Status::Complete);
        }

		void start()
		{
			_internalWorkingState = Status::OnEnter;
		}
	};
}

