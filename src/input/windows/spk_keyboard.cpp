#include "input/spk_keyboard.hpp"

namespace spk
{
    Keyboard::Mapping Keyboard::Mapping::createAzertyMapping()
    {
        Mapping result;


        result.bindKey(8, Backspace);
        result.bindKey(9, Tab);
        result.bindKey(12, Clear);
        result.bindKey(13, Return);
        result.bindKey(16, Shift);
        result.bindKey(17, Control);
        result.bindKey(18, Alt);
        result.bindKey(19, Pause);
        result.bindKey(20, Capslock);
        result.bindKey(27, Escape);
        result.bindKey(28, Convert);
        result.bindKey(29, Non_convert);
        result.bindKey(30, Accept);
        result.bindKey(31, Mode_change);
        result.bindKey(32, Space);
        result.bindKey(33, Prior);
        result.bindKey(34, Next);
        result.bindKey(35, End);
        result.bindKey(36, Home);
        result.bindKey(37, LeftArrow);
        result.bindKey(38, UpArrow);
        result.bindKey(39, RightArrow);
        result.bindKey(40, DownArrow);
        result.bindKey(41, Select);
        result.bindKey(42, Print);
        result.bindKey(43, Execute);
        result.bindKey(44, Snapshot);
        result.bindKey(45, Insert);
        result.bindKey(46, Delete);
        result.bindKey(47, Help);
        result.bindKey(48, Key0);
        result.bindKey(49, Key1);
        result.bindKey(50, Key2);
        result.bindKey(51, Key3);
        result.bindKey(52, Key4);
        result.bindKey(53, Key5);
        result.bindKey(54, Key6);
        result.bindKey(55, Key7);
        result.bindKey(56, Key8);
        result.bindKey(57, Key9);
        result.bindKey(65, A);
        result.bindKey(66, B);
        result.bindKey(67, C);
        result.bindKey(68, D);
        result.bindKey(69, E);
        result.bindKey(70, F);
        result.bindKey(71, G);
        result.bindKey(72, H);
        result.bindKey(73, I);
        result.bindKey(74, J);
        result.bindKey(75, K);
        result.bindKey(76, L);
        result.bindKey(77, M);
        result.bindKey(78, N);
        result.bindKey(79, O);
        result.bindKey(80, P);
        result.bindKey(81, Q);
        result.bindKey(82, R);
        result.bindKey(83, S);
        result.bindKey(84, T);
        result.bindKey(85, U);
        result.bindKey(86, V);
        result.bindKey(87, W);
        result.bindKey(88, X);
        result.bindKey(89, Y);
        result.bindKey(90, Z);
        result.bindKey(91, LeftWindows);
        result.bindKey(92, RightWindows);
        result.bindKey(93, App);
        result.bindKey(95, Sleep);
        result.bindKey(96, Numpad0);
        result.bindKey(97, Numpad1);
        result.bindKey(98, Numpad2);
        result.bindKey(99, Numpad3);
        result.bindKey(100, Numpad4);
        result.bindKey(101, Numpad5);
        result.bindKey(102, Numpad6);
        result.bindKey(103, Numpad7);
        result.bindKey(104, Numpad8);
        result.bindKey(105, Numpad9);
        result.bindKey(106, NumpadMultiply);
        result.bindKey(107, NumpadPlus);
        result.bindKey(108, NumpadSeparator);
        result.bindKey(109, NumpadMinus);
        result.bindKey(110, NumpadDecimal);
        result.bindKey(111, NumpadDivide);
        result.bindKey(112, F1);
        result.bindKey(113, F2);
        result.bindKey(114, F3);
        result.bindKey(115, F4);
        result.bindKey(116, F5);
        result.bindKey(117, F6);
        result.bindKey(118, F7);
        result.bindKey(119, F8);
        result.bindKey(120, F9);
        result.bindKey(121, F10);
        result.bindKey(122, F11);
        result.bindKey(123, F12);
        result.bindKey(124, F13);
        result.bindKey(125, F14);
        result.bindKey(126, F15);
        result.bindKey(127, F16);
        result.bindKey(128, F17);
        result.bindKey(129, F18);
        result.bindKey(130, F19);
        result.bindKey(131, F20);
        result.bindKey(132, F21);
        result.bindKey(133, F22);
        result.bindKey(134, F23);
        result.bindKey(135, F24);
        result.bindKey(144, NumLock);
        result.bindKey(145, Scroll);
        result.bindKey(160, LeftShift);
        result.bindKey(161, RightShift);
        result.bindKey(162, LeftControl);
        result.bindKey(163, RightControl);
        result.bindKey(164, LeftMenu);
        result.bindKey(165, RightMenu);
        result.bindKey(186, SemiColon);
        result.bindKey(187, Plus);
        result.bindKey(188, Comma);
        result.bindKey(189, Minus);
        result.bindKey(190, Period);
        result.bindKey(191, QuestionMark);
        result.bindKey(192, Tilde);
        result.bindKey(219, LeftBracket);
        result.bindKey(220, VerticalLine);
        result.bindKey(221, RightBracket);
        result.bindKey(222, Quote);
        result.bindKey(223, Unknow);
        result.bindKey(226, AngleBracket);
        result.bindKey(229, Process);

        return result;
    }

    Keyboard::Mapping Keyboard::Mapping::createQwertyMapping()
    {
        Mapping result;

        // TODO: Fill the mapping for AZERTY keyboard
        
        return result;
    }
}