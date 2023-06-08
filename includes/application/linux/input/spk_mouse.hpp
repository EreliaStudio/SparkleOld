#pragma once

#include "design_pattern/spk_singleton.hpp"
#include "application/linux/spk_define.hpp"
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
			Left,
			Middle,
			Right,
			Button3,
			Button4
		};
		static const size_t NB_BUTTON = 5;

	private:
		InputStatus _buttons[NB_BUTTON];

		Vector2Int _position;
		Vector2Int _deltaPosition;
		Vector2Int _wheel;

	private:
		Mouse();

		void setMousePosition(Vector2Int p_newPosition);
		void pressButton(const Button& p_button);
		void releaseButton(const Button& p_button);
		void editWheelPosition(const Vector2Int& p_delta);
		void update();

	public:
		void place(const Vector2Int& p_position)
		{
			
		}

		const InputStatus& getbutton(const Button& p_button) const
		{
			return (_buttons[static_cast<size_t>(p_button)]);
		}
		const Vector2Int& position() const { return (_position); }
		const Vector2Int& deltaPosition() const { return (_deltaPosition); }
		const Vector2Int& wheel() const { return (_wheel); }
	};

	std::wstring to_wstring(const Mouse::Button& p_button);
}