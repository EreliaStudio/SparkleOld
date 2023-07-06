#pragma once

#include "input/spk_input.h"
#include "input/spk_mouse.h"

namespace spk
{
	class MouseInput : public IInput
	{
	private:
		spk::Mouse::Button _button;
		spk::InputStatus _expectedStatus;

		bool _isExecutionNeeded() {
			return (spk::Mouse::instance()->getButton(_button) == _expectedStatus);
		}

	public:
		template <typename Funct, typename... Args>
		MouseInput(spk::Mouse::Button p_button, spk::InputStatus p_expectedStatus, unsigned long p_inputDelay, Funct&& p_funct, Args&&... p_args) :
			IInput(p_inputDelay, std::forward<Funct>(p_funct), std::forward<Args>(p_args)...),
			_button(p_button),
			_expectedStatus(p_expectedStatus)
		{

		}
	};
}

