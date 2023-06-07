#pragma once

#include "design_pattern/spk_singleton.hpp"
#include "application/system/spk_define.hpp"

namespace spk
{
	class Keyboard : public spk::Singleton<Keyboard>
	{
		friend class spk::Singleton<Keyboard>;
		friend class KeybaordModule;

	public:
		enum Key
		{

		};
		static const size_t NB_KEYS = 256;

		InputStatus _keys[NB_KEYS];

	private:
		Keyboard()
		{
		}

		void pressKey(const Key &p_key)
		{
			_keys[static_cast<size_t>(p_key)] = InputStatus::Pressed;
		}
		void releaseKey(const Key &p_key)
		{
			_keys[static_cast<size_t>(p_key)] = InputStatus::Released;
		}

		void update()
		{
			for (size_t i = 0; i < NB_KEYS; i++)
			{
				if (_keys[i] == InputStatus::Pressed)
					_keys[i] = InputStatus::Down;
				else if (_keys[i] == InputStatus::Released)
					_keys[i] = InputStatus::Up;
			}
		}

	public:
		const InputStatus &key(const Key &p_key) const
		{
			return (_keys[static_cast<size_t>(p_key)]);
		}
	};
}