#include "input/spk_input_group.hpp"
#include "spk_basic_functions.hpp"

namespace spk
{
	void InputGroup::addInput(Input* p_input)
	{
		auto it = std::find(_inputs.begin(), _inputs.end(), p_input);
		if (it != _inputs.end())
		{
			spk::throwException(L"Input already inside the InputGroup");
		}

		_inputs.push_back(p_input);
	}
	
	void InputGroup::removeInput(Input* p_input)
	{
		auto it = std::find(_inputs.begin(), _inputs.end(), p_input);
		if (it != _inputs.end())
		{
			_inputs.erase(it);
		}
		else
		{
			spk::throwException(L"Input not contained inside InputGroup");
		}
	}

	void InputGroup::update()
	{
		for (auto& input : _inputs)
		{
			input->update();
		}
	}
}