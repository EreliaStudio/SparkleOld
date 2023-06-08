#include "application/system/spk_keyboard.hpp"

namespace spk
{
	Keyboard::Keyboard()
	{
		
	}

	void Keyboard::pressKey(const Key &p_key)
	{
		_keys[static_cast<size_t>(p_key)] = InputStatus::Pressed;
	}
	void Keyboard::releaseKey(const Key &p_key)
	{
		_keys[static_cast<size_t>(p_key)] = InputStatus::Released;
	}

	void Keyboard::update()
	{
		for (size_t i = 0; i < NB_KEYS; i++)
		{
			if (_keys[i] == InputStatus::Pressed)
				_keys[i] = InputStatus::Down;
			else if (_keys[i] == InputStatus::Released)
				_keys[i] = InputStatus::Up;
		}
	}

	const std::wstring& Keyboard::keyToString(const Keyboard::Key& p_key)
	{
		if (Keyboard::KeyToStringMap.count(p_key) == 0)
			return (Keyboard::UnknowKeyName);
		else 
			return (Keyboard::KeyToStringMap.at(p_key));
	}

	const std::wstring& to_wstring(const Keyboard::Key& p_key)
	{
		return (Keyboard::keyToString(p_key));
	}
}