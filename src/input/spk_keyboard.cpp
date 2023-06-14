#include "input/spk_keyboard.hpp"

namespace spk
{
	std::map<Keyboard::Key, std::wstring> Keyboard::KeyToStringMap = {
		{Keyboard::Key::Backspace, L"Backspace"},
		{Keyboard::Key::Tab, L"Tab"},
		{Keyboard::Key::Clear, L"Clear"},
		{Keyboard::Key::Return, L"Return"},
		{Keyboard::Key::Shift, L"Shift"},
		{Keyboard::Key::Control, L"Control"},
		{Keyboard::Key::Alt, L"Alt"},
		{Keyboard::Key::Pause, L"Pause"},
		{Keyboard::Key::Capslock, L"Capslock"},
		{Keyboard::Key::Escape, L"Escape"},
		{Keyboard::Key::Convert, L"Convert"},
		{Keyboard::Key::Non_convert, L"Non_convert"},
		{Keyboard::Key::Accept, L"Accept"},
		{Keyboard::Key::Mode_change, L"Mode_change"},
		{Keyboard::Key::Space, L"Space"},
		{Keyboard::Key::Prior, L"Prior"},
		{Keyboard::Key::Next, L"Next"},
		{Keyboard::Key::End, L"End"},
		{Keyboard::Key::Home, L"Home"},
		{Keyboard::Key::LeftArrow, L"LeftArrow"},
		{Keyboard::Key::UpArrow, L"UpArrow"},
		{Keyboard::Key::RightArrow, L"RightArrow"},
		{Keyboard::Key::DownArrow, L"DownArrow"},
		{Keyboard::Key::Select, L"Select"},
		{Keyboard::Key::Print, L"Print"},
		{Keyboard::Key::Execute, L"Execute"},
		{Keyboard::Key::Snapshot, L"Snapshot"},
		{Keyboard::Key::Insert, L"Insert"},
		{Keyboard::Key::Delete, L"Delete"},
		{Keyboard::Key::Help, L"Help"},
		{Keyboard::Key::Key0, L"Key0"},
		{Keyboard::Key::Key1, L"Key1"},
		{Keyboard::Key::Key2, L"Key2"},
		{Keyboard::Key::Key3, L"Key3"},
		{Keyboard::Key::Key4, L"Key4"},
		{Keyboard::Key::Key5, L"Key5"},
		{Keyboard::Key::Key6, L"Key6"},
		{Keyboard::Key::Key7, L"Key7"},
		{Keyboard::Key::Key8, L"Key8"},
		{Keyboard::Key::Key9, L"Key9"},
		{Keyboard::Key::A, L"A"},
		{Keyboard::Key::B, L"B"},
		{Keyboard::Key::C, L"C"},
		{Keyboard::Key::D, L"D"},
		{Keyboard::Key::E, L"E"},
		{Keyboard::Key::F, L"F"},
		{Keyboard::Key::G, L"G"},
		{Keyboard::Key::H, L"H"},
		{Keyboard::Key::I, L"I"},
		{Keyboard::Key::J, L"J"},
		{Keyboard::Key::K, L"K"},
		{Keyboard::Key::L, L"L"},
		{Keyboard::Key::M, L"M"},
		{Keyboard::Key::N, L"N"},
		{Keyboard::Key::O, L"O"},
		{Keyboard::Key::P, L"P"},
		{Keyboard::Key::Q, L"Q"},
		{Keyboard::Key::R, L"R"},
		{Keyboard::Key::S, L"S"},
		{Keyboard::Key::T, L"T"},
		{Keyboard::Key::U, L"U"},
		{Keyboard::Key::V, L"V"},
		{Keyboard::Key::W, L"W"},
		{Keyboard::Key::X, L"X"},
		{Keyboard::Key::Y, L"Y"},
		{Keyboard::Key::Z, L"Z"},
		{Keyboard::Key::LeftWindows, L"LeftWindows"},
		{Keyboard::Key::RightWindows, L"RightWindows"},
		{Keyboard::Key::App, L"App"},
		{Keyboard::Key::Sleep, L"Sleep"},
		{Keyboard::Key::Numpad0, L"Numpad0"},
		{Keyboard::Key::Numpad1, L"Numpad1"},
		{Keyboard::Key::Numpad2, L"Numpad2"},
		{Keyboard::Key::Numpad3, L"Numpad3"},
		{Keyboard::Key::Numpad4, L"Numpad4"},
		{Keyboard::Key::Numpad5, L"Numpad5"},
		{Keyboard::Key::Numpad6, L"Numpad6"},
		{Keyboard::Key::Numpad7, L"Numpad7"},
		{Keyboard::Key::Numpad8, L"Numpad8"},
		{Keyboard::Key::Numpad9, L"Numpad9"},
		{Keyboard::Key::NumpadMultiply, L"NumpadMultiply"},
		{Keyboard::Key::NumpadPlus, L"NumpadPlus"},
		{Keyboard::Key::NumpadSeparator, L"NumpadSeparator"},
		{Keyboard::Key::NumpadMinus, L"NumpadMinus"},
		{Keyboard::Key::NumpadDecimal, L"NumpadDecimal"},
		{Keyboard::Key::NumpadDivide, L"NumpadDivide"},
		{Keyboard::Key::F1, L"F1"},
		{Keyboard::Key::F2, L"F2"},
		{Keyboard::Key::F3, L"F3"},
		{Keyboard::Key::F4, L"F4"},
		{Keyboard::Key::F5, L"F5"},
		{Keyboard::Key::F6, L"F6"},
		{Keyboard::Key::F7, L"F7"},
		{Keyboard::Key::F8, L"F8"},
		{Keyboard::Key::F9, L"F9"},
		{Keyboard::Key::F10, L"F10"},
		{Keyboard::Key::F11, L"F11"},
		{Keyboard::Key::F12, L"F12"},
		{Keyboard::Key::F13, L"F13"},
		{Keyboard::Key::F14, L"F14"},
		{Keyboard::Key::F15, L"F15"},
		{Keyboard::Key::F16, L"F16"},
		{Keyboard::Key::F17, L"F17"},
		{Keyboard::Key::F18, L"F18"},
		{Keyboard::Key::F19, L"F19"},
		{Keyboard::Key::F20, L"F20"},
		{Keyboard::Key::F21, L"F21"},
		{Keyboard::Key::F22, L"F22"},
		{Keyboard::Key::F23, L"F23"},
		{Keyboard::Key::F24, L"F24"},
		{Keyboard::Key::NumLock, L"NumLock"},
		{Keyboard::Key::Scroll, L"Scroll"},
		{Keyboard::Key::LeftShift, L"LeftShift"},
		{Keyboard::Key::RightShift, L"RightShift"},
		{Keyboard::Key::LeftControl, L"LeftControl"},
		{Keyboard::Key::RightControl, L"RightControl"},
		{Keyboard::Key::LeftAlt, L"LeftAlt"},
		{Keyboard::Key::RightAlt, L"RightAlt"},
		{Keyboard::Key::LeftMenu, L"LeftMenu"},
		{Keyboard::Key::RightMenu, L"RightMenu"},
		{Keyboard::Key::SemiColon, L"SemiColon"},
		{Keyboard::Key::Plus, L"Plus"},
		{Keyboard::Key::Comma, L"Comma"},
		{Keyboard::Key::Minus, L"Minus"},
		{Keyboard::Key::Period, L"Period"},
		{Keyboard::Key::QuestionMark, L"QuestionMark"},
		{Keyboard::Key::Tilde, L"Tilde"},
		{Keyboard::Key::LeftBracket, L"LeftBracket"},
		{Keyboard::Key::VerticalLine, L"VerticalLine"},
		{Keyboard::Key::RightBracket, L"RightBracket"},
		{Keyboard::Key::Quote, L"Quote"},
		{Keyboard::Key::Unknow, L"Unknow"},
		{Keyboard::Key::AngleBracket, L"AngleBracket"},
		{Keyboard::Key::Process, L"Process"},
		{Keyboard::Key::Backslash, L"Backslash"},
		{Keyboard::Key::Slash, L"Slash"},
		{Keyboard::Key::NumpadReturn, L"NumpadReturn"}};
	const std::wstring Keyboard::UnknowKeyName = L"Unknow key";

	Keyboard::Mapping::Mapping()
	{
		for (int i = 0; i < 255; ++i)
			keys[i] = Key::Unknown;
	}

	void Keyboard::Mapping::bindKey(const size_t& p_index, Key p_key)
	{
		keys[p_index] = p_key;
	}

	Keyboard::Key &Keyboard::Mapping::operator[](const size_t &p_index)
	{
		return (keys[p_index]);
	}

	Keyboard::Keyboard() :
		_layout(Layout::Qwerty),
		_mapping{
				Mapping::createAzertyMapping(),
				Mapping::createQwertyMapping()
			}
	{
	}

	void Keyboard::pressKey(const uint32_t& p_key)
	{
		Key &keyValue = _mapping[static_cast<int>(_layout)][p_key];

		_keys[static_cast<size_t>(keyValue)] = InputStatus::Pressed;
	}
	void Keyboard::releaseKey(const uint32_t& p_key)
	{
		Key &keyValue = _mapping[static_cast<int>(_layout)][p_key];

		_keys[static_cast<size_t>(keyValue)] = InputStatus::Released;
	}

	void Keyboard::update()
	{
		for (size_t i = 0; i < Key::SIZE; i++)
		{
			if (_keys[i] == InputStatus::Pressed)
				_keys[i] = InputStatus::Down;
			else if (_keys[i] == InputStatus::Released)
				_keys[i] = InputStatus::Up;
		}
	}

	const std::wstring &Keyboard::keyToString(const Keyboard::Key &p_key)
	{
		if (Keyboard::KeyToStringMap.count(p_key) == 0)
			return (Keyboard::UnknowKeyName);
		else
			return (Keyboard::KeyToStringMap.at(p_key));
	}

	const std::wstring &to_wstring(const Keyboard::Key &p_key)
	{
		return (Keyboard::keyToString(p_key));
	}
}