#pragma once

#include "input/spk_input.hpp"
#include <vector>

namespace spk
{
	class InputManager
	{
	private:
		std::vector<Input*> _inputs;

	public:
		void addInput(Input* p_input);
		void removeInput(Input* p_input);
		void update();
	};
}