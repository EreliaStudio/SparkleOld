#include "input/spk_input_manager.hpp"
#include "spk_basic_functions.hpp"

namespace spk
{
	void InputManager::addInput(Input* p_input)
	{
		auto it = std::find(_inputs.begin(), _inputs.end(), p_input);
		if (it != _inputs.end())
		{
			spk::throwException(L"Input already inside the InputManager");
		}

		_inputs.push_back(p_input);
	}
	
	void InputManager::removeInput(Input* p_input)
	{
		auto it = std::find(_inputs.begin(), _inputs.end(), p_input);
		if (it != _inputs.end())
		{
			_inputs.erase(it);
		}
		else
		{
			spk::throwException(L"Input not contained inside InputManager");
		}
	}

	void InputManager::update()
	{
		for (auto& input : _inputs)
		{
			input->update();
		}
	}
}