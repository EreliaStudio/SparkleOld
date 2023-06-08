#pragma once

#include "design_pattern/spk_singleton.hpp"
#include "application/system/spk_define.hpp"
#include <map>

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
            Shift,
            Control,
            Alt,
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
            End,
            Home,
            LeftArrow,
            UpArrow,
            RightArrow,
            DownArrow,
            Select,
            Print,
            Execute,
            Snapshot,
            Insert,
            Delete,
            Help,
            Key0,
            Key1,
            Key2,
            Key3,
            Key4,
            Key5,
            Key6,
            Key7,
            Key8,
            Key9,
            A,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            J,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z,
            LeftWindows,
            RightWindows,
            App,
            Sleep,
            Numpad0,
            Numpad1,
            Numpad2,
            Numpad3,
            Numpad4,
            Numpad5,
            Numpad6,
            Numpad7,
            Numpad8,
            Numpad9,
            NumpadMultiply,
            NumpadPlus,
            NumpadSeparator,
            NumpadMinus,
            NumpadDecimal,
            NumpadDivide,
            F1,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            F11,
            F12,
            F13,
            F14,
            F15,
            F16,
            F17,
            F18,
            F19,
            F20,
            F21,
            F22,
            F23,
            F24,
            NumLock,
            Scroll,
            LeftShift,
            RightShift,
            LeftControl,
            RightControl,
            LeftAlt,
            RightAlt,
            LeftMenu,
            RightMenu,
            SemiColon,
            Plus,
            Comma,
            Minus,
            Period,
            QuestionMark,
            Tilde,
            LeftBracket,
            VerticalLine,
            RightBracket,
            Quote,
            Unknow,
            AngleBracket,
            Process,
            Backslash,
            Slash,
            NumpadReturn,
            Equal
        };

	enum Layout
	{
		Azerty,
		Qwerty
	};

	struct Mapping
	{
		Key keys[255] = {};

		Mapping()
		{
			for (int i = 0; i < 255; ++i)
				keys[i] = Key::Unknown;
		}

		void setKey(size_t p_index, Key p_key)
		{
			keys[p_index] = p_key;
		}

        Key& operator[](const size_t& p_index)
        {
            return (keys[p_index]);
        }

		static Mapping createAzertyMapping()
		{
			Mapping result;

			// TODO: Fill the mapping for AZERTY keyboard

			return result;
		}

		static Mapping createQwertyMapping()
		{
			Mapping result;

            result.setKey(9, Escape);
            result.setKey(10, Key1);
            result.setKey(11, Key2);
            result.setKey(12, Key3);
            result.setKey(13, Key4);
            result.setKey(14, Key5);
            result.setKey(15, Key6);
            result.setKey(16, Key7);
            result.setKey(17, Key8);
            result.setKey(18, Key9);
            result.setKey(19, Key0);
            result.setKey(20, Minus);
            result.setKey(21, Equal);
            result.setKey(22, Backspace);
            result.setKey(23, Tab);
            result.setKey(24, Q);
			result.setKey(25, W);
			result.setKey(26, E);
			result.setKey(27, R);
			result.setKey(28, T);
			result.setKey(29, Y);
			result.setKey(30, U);
			result.setKey(31, I);
			result.setKey(32, O);
			result.setKey(33, P);
			result.setKey(34, LeftBracket);
			result.setKey(35, RightBracket);
			result.setKey(36, Return);
			result.setKey(37, LeftControl);
			result.setKey(38, A);
			result.setKey(39, S);
			result.setKey(40, D);
			result.setKey(41, F);
			result.setKey(42, G);
			result.setKey(43, H);
			result.setKey(44, J);
			result.setKey(45, K);
			result.setKey(46, L);
			result.setKey(47, SemiColon);
			result.setKey(48, Quote);
			result.setKey(49, Tilde);
			result.setKey(50, LeftShift);
			result.setKey(51, Backslash);
			result.setKey(52, Z);
			result.setKey(53, X);
			result.setKey(54, C);
			result.setKey(55, V);
			result.setKey(56, B);
			result.setKey(57, N);
			result.setKey(58, M);
			result.setKey(59, Comma);
			result.setKey(60, Period);
			result.setKey(61, Slash);
			result.setKey(62, RightShift);
			result.setKey(63, NumpadMultiply);
			result.setKey(64, LeftAlt);
			result.setKey(65, Space);
			result.setKey(66, Capslock);
			result.setKey(67, F1);
			result.setKey(68, F2);
			result.setKey(69, F3);
			result.setKey(70, F4);
			result.setKey(71, F5);
			result.setKey(72, F6);
			result.setKey(73, F7);
			result.setKey(74, F8);
			result.setKey(75, F9);
			result.setKey(76, F10);
			result.setKey(77, NumLock);
			result.setKey(79, Numpad7);
			result.setKey(80, Numpad8);
			result.setKey(81, Numpad9);
			result.setKey(82, NumpadMinus);
			result.setKey(83, Numpad4);
			result.setKey(84, Numpad5);
			result.setKey(85, Numpad6);
			result.setKey(86, NumpadPlus);
			result.setKey(87, Numpad1);
			result.setKey(88, Numpad2);
			result.setKey(89, Numpad3);
			result.setKey(90, Numpad0);
			result.setKey(91, NumpadDecimal);
			result.setKey(95, F11);
			result.setKey(96, F12);
			result.setKey(104, NumpadReturn);
			result.setKey(105, RightControl);
			result.setKey(106, NumpadDivide);
			result.setKey(108, RightAlt);
			result.setKey(110, Home);
			result.setKey(111, UpArrow);
			result.setKey(113, LeftArrow);
			result.setKey(114, RightArrow);
			result.setKey(115, End);
			result.setKey(116, DownArrow);
			result.setKey(118, Insert);
			result.setKey(119, Clear);

			return result;
		}
	};

	private:
    static inline std::map<Keyboard::Key, std::wstring> KeyToStringMap = {
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
            {Keyboard::Key::NumpadReturn, L"NumpadReturn"}
        };
		static inline const std::wstring UnknowKeyName = L"Unknow key";
		static const size_t NB_KEYS = 256;

		Layout _layout = Layout::Qwerty;
		Mapping _mapping[2] = {
            Mapping::createAzertyMapping(),
            Mapping::createQwertyMapping()
        };
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