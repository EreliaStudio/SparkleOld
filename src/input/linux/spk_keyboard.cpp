#include "input/spk_keyboard.hpp"

namespace spk
{
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
}