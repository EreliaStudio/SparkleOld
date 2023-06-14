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
			Stopped,
			OnEnter,
			Running,
			OnExit,
			Complete
		};

		Status _internalWorkingState;

		void _update();

		void _moveNextStep();
		void _moveNextStepAndExecute();

		void _start();

	protected:
		virtual void _onEnter() = 0;
		virtual void _execute() = 0;
		virtual void _onExit() = 0;

	public:
		Activity();

		const Status& state() const;
		bool isCompleted() const;
	};
}

