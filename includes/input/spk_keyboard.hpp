#pragma once

#include "design_pattern/spk_singleton.hpp"
#include "input/spk_input_status.hpp"
#include <map>
#include <vector>
#include <string>

namespace spk
{
	class Keyboard : public spk::Singleton<Keyboard>
	{
		friend class spk::Singleton<Keyboard>;
		friend class KeyboardModule;

	public:
		enum Key
        {
            Unknown = -1,
            Backspace,
            Tab,
            Clear,
            Return,
            Pause,
            Capslock,
            Escape,
            Convert,
            Non_convert,
            Accept,
            Mode_change,
            Space,
            Prior,
            Next,
            End, Home,
            LeftArrow, UpArrow, RightArrow, DownArrow,
            Select,
            Print,
            Execute,
            Snapshot,
            Insert, Delete,
            Help,
            Key0, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9, 
            A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			LeftWindows, RightWindows,
            App,
            Sleep,
            Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
            NumpadMultiply, NumpadPlus, NumpadMinus, NumpadDivide,
            NumpadSeparator, NumpadDecimal, NumpadReturn,
            NumLock,
            F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
			F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
            Scroll,
            Shift, LeftShift, RightShift,
            Control, LeftControl, RightControl,
            Alt, LeftAlt, RightAlt,
            Menu, LeftMenu, RightMenu,
            Plus, Minus, Equal,
            Comma, Period, SemiColon,
            QuestionMark,
            Tilde,
            LeftBracket, RightBracket,
            VerticalLine,
            Quote,
            Unknow,
            AngleBracket,
            Process,
            Backslash, Slash
        };

        enum Layout
        {
            Azerty,
            Qwerty
        };

    private:
        static const size_t NB_KEYS = 256;

        struct Mapping
        {
            Key keys[NB_KEYS];

            Mapping();
            void bindKey(size_t p_index, Key p_key);
            void bindKeys(size_t p_index, std::vector<Key> p_keys);
            Key& operator[](const size_t& p_index);

            static Mapping createAzertyMapping();
            static Mapping createQwertyMapping();
        };

        static std::map<Keyboard::Key, std::wstring> KeyToStringMap;
		static const std::wstring UnknowKeyName;

		Layout _layout;
		Mapping _mapping[2];
		InputStatus _keys[NB_KEYS];

		Keyboard();
		
		void pressKey(uint8_t p_key);
		void releaseKey(uint8_t p_key);

		void update();

	public:
		void setLayout(const Layout& p_layout) { _layout = p_layout; }

		const InputStatus &key(const Key &p_key) const
		{
			return (_keys[static_cast<size_t>(p_key)]);
		}

		static const std::wstring& keyToString(const Keyboard::Key& p_key);
	};

	const std::wstring& to_wstring(const Keyboard::Key& p_key);
}