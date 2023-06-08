#pragma once

#include "design_pattern/spk_singleton.hpp"
#include "application/system/spk_define.hpp"
#include "math/spk_vector2.hpp"
#include "iostream/spk_iostream.hpp"

namespace spk
{
	class Mouse : public spk::Singleton<Mouse>
	{
		friend class spk::Singleton<Mouse>;
		friend class MouseModule;

	public:
		enum Button
		{
			Left = 0,
			Middle = 1,
			Right = 2
		};

	private:
		InputStatus _buttons[3] = {
			spk::InputStatus::Up,
			spk::InputStatus::Up,
			spk::InputStatus::Up};

		Vector2Int _position;
		Vector2Int _deltaPosition;
		float _wheel;

	private:
		Mouse();

		void setMousePosition(Vector2Int p_newPosition);
		void pressButton(const Button &p_button);
		void releaseButton(const Button &p_button);
		void editWheelPosition(const float &p_delta);
		void update();

	public:
		const InputStatus &getbutton(const Button &p_button) const
		{
			return (_buttons[static_cast<size_t>(p_button)]);
		}
		const Vector2Int &position() const { return (_position); }
		const Vector2Int &deltaPosition() const { return (_deltaPosition); }
		float wheel() const { return (_wheel); }
	};

	std::wstring to_wstring(const Mouse::Button &p_button);
}