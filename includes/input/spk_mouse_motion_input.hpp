#pragma once

#include "input/spk_input.h"
#include "input/spk_mouse.h"

namespace spk
{
	class MouseMovementInput : public IInput
	{
	private:
		bool _isExecutionNeeded() {
			return (true);
		}

	public:
		template <typename Funct, typename... Args>
		MouseMovementInput(unsigned long inputDelay, Funct&& funct, Args&&... args)
			IInput(p_inputDelay, std::forward<Funct>(p_funct), std::forward<Args>(p_args)...)
	};
}