#pragma once

#include <functional>
#include "system/spk_timer.hpp"

namespace spk
{
	class IInput
	{
	protected:
		spk::Timer _timer;
		std::function<void()> _funct;

		virtual bool _isExecutionNeeded() = 0;
		virtual bool _skipCondition() {
			return (true);
		}

	public:
		template <typename Funct, typename... Args>
		IInput(unsigned long p_inputDelay, Funct&& p_funct, Args&&... p_args) :
			_timer(p_inputDelay),
			_funct(std::bind(std::forward<Funct>(p_funct), std::forward<Args>(p_args)...))
		{

		}
		virtual void update()
		{
			if (_skipCondition() == true && _timer.isRunning())
				return;

			if (_isExecutionNeeded() == true) {
				_funct();
				_timer.start();
			}
		}
	};
}