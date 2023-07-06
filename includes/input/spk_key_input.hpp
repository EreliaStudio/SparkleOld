#pragma once

#include "input/spk_input.h"
#include "input/spk_keyboard.h"

namespace spk
{
	class KeyInput : public IInput
	{
	private:
		spk::Keyboard::Key _key;
		spk::InputStatus _expectedStatus;

		bool _isExecutionNeeded() {
			return (spk::Keyboard::instance()->getKey(_key) == _expectedStatus);
		}

	public:
		template <typename Funct, typename... Args>
		KeyInput(spk::Keyboard::Key p_key, spk::InputStatus p_expectedStatus, unsigned long p_inputDelay, Funct&& p_funct, Args&&... p_args) :
			IInput(p_inputDelay, std::forward<Funct>(p_funct), std::forward<Args>(p_args)...),
			_key(p_key),
			_expectedStatus(p_expectedStatus)
		{

		}
	};
}

