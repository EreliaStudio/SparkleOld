#pragma once

#include "input/spk_input.hpp"
#include <vector>

namespace spk
{
	class InputGroup
	{
	private:
		std::vector<Input*> _inputs;

	public:
		void addInput(Input* p_input);
		void removeInput(Input* p_input);
		void update();
	};
}