#include "application/system/spk_mouse.hpp"

namespace spk
{
	Mouse::Mouse()
		: _position(0, 0),
		_deltaPosition(0, 0),
		_wheel(0)
	{}

    void Mouse::setMousePosition(Vector2Int p_newPosition)
    {
        _deltaPosition = p_newPosition - _position;
        _position = p_newPosition;
    }

    void Mouse::pressButton(const Button &p_button)
    {
        _buttons[static_cast<size_t>(p_button)] = InputStatus::Pressed;
    }

    void Mouse::releaseButton(const Button &p_button)
    {
        _buttons[static_cast<size_t>(p_button)] = InputStatus::Released;
    }

    void Mouse::editWheelPosition(const float &p_delta)
    {
        _wheel += p_delta;
    }

    void Mouse::update()
    {
        _deltaPosition.x = 0;
        _deltaPosition.y = 0;

        for (size_t i = 0; i < 3; i++)
        {
            if (_buttons[i] == InputStatus::Pressed)
            {
                _buttons[i] = InputStatus::Down;
            }
            else if (_buttons[i] == InputStatus::Released)
            {
                _buttons[i] = InputStatus::Up;
            }
        
            spk::cout << "Button " << to_wstring(Button(i)) << " -> " << to_wstring(_buttons[i]) << std::endl;
        }
    }

	std::wstring to_wstring(const Mouse::Button &p_button)
	{
		switch (p_button)
		{
		case Mouse::Button::Left:
			return (L"Left");
		case Mouse::Button::Middle:
			return (L"Middle");
		case Mouse::Button::Right:
			return (L"Right");
		}
		return (L"Unknown");
	}
}