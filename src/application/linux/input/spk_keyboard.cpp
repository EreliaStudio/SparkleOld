#include "application/linux/input/spk_keyboard.hpp"
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

    void Keyboard::Mapping::bindKey(size_t p_index, Key p_key)
    {
        keys[p_index] = p_key;
    }

    Keyboard::Key &Keyboard::Mapping::operator[](const size_t &p_index)
    {
        return (keys[p_index]);
    }

    Keyboard::Mapping Keyboard::Mapping::createAzertyMapping()
    {
        Mapping result;

        // TODO: Fill the mapping for AZERTY keyboard

        return result;
    }

    Keyboard::Mapping Keyboard::Mapping::createQwertyMapping()
    {
        Mapping result;

        result.bindKey(9, Escape);
        result.bindKey(10, Key1);
        result.bindKey(11, Key2);
        result.bindKey(12, Key3);
        result.bindKey(13, Key4);
        result.bindKey(14, Key5);
        result.bindKey(15, Key6);
        result.bindKey(16, Key7);
        result.bindKey(17, Key8);
        result.bindKey(18, Key9);
        result.bindKey(19, Key0);
        result.bindKey(20, Minus);
        result.bindKey(21, Equal);
        result.bindKey(22, Backspace);
        result.bindKey(23, Tab);
        result.bindKey(24, Q);
        result.bindKey(25, W);
        result.bindKey(26, E);
        result.bindKey(27, R);
        result.bindKey(28, T);
        result.bindKey(29, Y);
        result.bindKey(30, U);
        result.bindKey(31, I);
        result.bindKey(32, O);
        result.bindKey(33, P);
        result.bindKey(34, LeftBracket);
        result.bindKey(35, RightBracket);
        result.bindKey(36, Return);
        result.bindKey(37, LeftControl);
        result.bindKey(38, A);
        result.bindKey(39, S);
        result.bindKey(40, D);
        result.bindKey(41, F);
        result.bindKey(42, G);
        result.bindKey(43, H);
        result.bindKey(44, J);
        result.bindKey(45, K);
        result.bindKey(46, L);
        result.bindKey(47, SemiColon);
        result.bindKey(48, Quote);
        result.bindKey(49, Tilde);
        result.bindKey(50, LeftShift);
        result.bindKey(51, Backslash);
        result.bindKey(52, Z);
        result.bindKey(53, X);
        result.bindKey(54, C);
        result.bindKey(55, V);
        result.bindKey(56, B);
        result.bindKey(57, N);
        result.bindKey(58, M);
        result.bindKey(59, Comma);
        result.bindKey(60, Period);
        result.bindKey(61, Slash);
        result.bindKey(62, RightShift);
        result.bindKey(63, NumpadMultiply);
        result.bindKey(64, LeftAlt);
        result.bindKey(65, Space);
        result.bindKey(66, Capslock);
        result.bindKey(67, F1);
        result.bindKey(68, F2);
        result.bindKey(69, F3);
        result.bindKey(70, F4);
        result.bindKey(71, F5);
        result.bindKey(72, F6);
        result.bindKey(73, F7);
        result.bindKey(74, F8);
        result.bindKey(75, F9);
        result.bindKey(76, F10);
        result.bindKey(77, NumLock);
        result.bindKey(79, Numpad7);
        result.bindKey(80, Numpad8);
        result.bindKey(81, Numpad9);
        result.bindKey(82, NumpadMinus);
        result.bindKey(83, Numpad4);
        result.bindKey(84, Numpad5);
        result.bindKey(85, Numpad6);
        result.bindKey(86, NumpadPlus);
        result.bindKey(87, Numpad1);
        result.bindKey(88, Numpad2);
        result.bindKey(89, Numpad3);
        result.bindKey(90, Numpad0);
        result.bindKey(91, NumpadDecimal);
        result.bindKey(95, F11);
        result.bindKey(96, F12);
        result.bindKey(104, NumpadReturn);
        result.bindKey(105, RightControl);
        result.bindKey(106, NumpadDivide);
        result.bindKey(108, RightAlt);
        result.bindKey(110, Home);
        result.bindKey(111, UpArrow);
        result.bindKey(113, LeftArrow);
        result.bindKey(114, RightArrow);
        result.bindKey(115, End);
        result.bindKey(116, DownArrow);
        result.bindKey(118, Insert);
        result.bindKey(119, Clear);

        return result;
    }


    Keyboard::Keyboard() :
        _layout(Layout::Qwerty),
        _mapping{
                Mapping::createAzertyMapping(),
                Mapping::createQwertyMapping()
            }
    {
    }

    void Keyboard::pressKey(uint8_t p_key)
    {
        Key &keyValue = _mapping[static_cast<int>(_layout)][p_key];

        _keys[static_cast<size_t>(keyValue)] = InputStatus::Pressed;
    }
    void Keyboard::releaseKey(uint8_t p_key)
    {
        Key &keyValue = _mapping[static_cast<int>(_layout)][p_key];

        _keys[static_cast<size_t>(keyValue)] = InputStatus::Released;
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