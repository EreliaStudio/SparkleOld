#include "input/spk_mouse.hpp"
#include "graphics/spk_window.hpp"

namespace spk
{
	Mouse::Mouse() :
		_position(0, 0),
		_deltaPosition(0, 0),
		_wheel(0)
	{
		for (size_t i = 0; i < NB_BUTTON; i++)
		{
			_buttons[i] = InputState::Up;
		}
	}

	void Mouse::setMousePosition(const Vector2Int& p_newPosition)
	{
		if (_deltaPosition == INITIAL_DELTA_POSITION)
			_deltaPosition = 0;
		else
			_deltaPosition = p_newPosition - _position;
		_position = p_newPosition;
	}

	void Mouse::pressButton(const Button& p_button)
	{
		if (_buttons[static_cast<size_t>(p_button)] != InputState::Down)
			_buttons[static_cast<size_t>(p_button)] = InputState::Pressed;
	}

	void Mouse::releaseButton(const Button& p_button)
	{
		if (_buttons[static_cast<size_t>(p_button)] != InputState::Up)
			_buttons[static_cast<size_t>(p_button)] = InputState::Released;
	}

	void Mouse::editWheelPosition(const Vector2Int& p_delta)
	{
		_wheel += p_delta;
	}

	void Mouse::update()
	{
		_deltaPosition.x = 0;
		_deltaPosition.y = 0;

		for (size_t i = 0; i < NB_BUTTON; i++)
		{
			if (_buttons[i] == InputState::Pressed)
			{
				_buttons[i] = InputState::Down;
			}
			else if (_buttons[i] == InputState::Released)
			{
				_buttons[i] = InputState::Up;
			}
		}
	}

	void Mouse::place(const Vector2Int& p_position)
	{	
		SetCursorPos(spk::Window::instance()->anchor().x + p_position.x, spk::Window::instance()->anchor().y + p_position.y);
		_position = p_position;
		_deltaPosition = 0;
	}

	std::wstring to_wstring(const Mouse::Button& p_button)
	{
		switch (p_button)
		{
		case Mouse::Button::Left:
			return (L"Left");
		case Mouse::Button::Middle:
			return (L"Middle");
		case Mouse::Button::Right:
			return (L"Right");
		case Mouse::Button::Button3:
			return (L"Button3");
		case Mouse::Button::Button4:
			return (L"Button4");
		}
		return (L"Unknown");
	}
}