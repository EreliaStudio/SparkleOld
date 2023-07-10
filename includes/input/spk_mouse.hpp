#pragma once

#include "design_pattern/spk_singleton.hpp"
#include "input/spk_input_status.hpp"
#include "math/spk_vector2.hpp"

namespace spk
{
	/**
	* @class Mouse
	* @brief Singleton class for Mouse.
	*
	* Represents the mouse device, providing functionalities to manage mouse actions such as button pressing, position, wheel movement, etc.
	*/
	class Mouse : public spk::Singleton<Mouse>
	{
		friend class spk::Singleton<Mouse>;
		friend class MouseModule;

	public:
		/**
		* @enum Button
		* @brief Enum for mouse buttons.
		*/
		enum Button
		{
			Left,
			Middle,
			Right,
			Button3,
			Button4
		};

		static const size_t NB_BUTTON = 5; ///< Number of buttons on a mouse.

	private:
		InputState _buttons[NB_BUTTON]; ///< Array storing the state of each button.

		Vector2Int _position; ///< Current position of the mouse.
		Vector2Int _deltaPosition; ///< Change in position of the mouse.
		Vector2Int _wheel; ///< Mouse wheel movement.

	private:
		/**
		* @brief Constructs a Mouse object.
		*/
		Mouse();

		/**
		* @brief Sets the position of the mouse.
		*
		* @param p_newPosition The new position to set for the mouse.
		*/
		void setMousePosition(const Vector2Int& p_newPosition);

		/**
		* @brief Simulates a mouse button press.
		*
		* @param p_button The button to simulate a press for.
		*/
		void pressButton(const Button& p_button);

		/**
		* @brief Simulates a mouse button release.
		*
		* @param p_button The button to simulate a release for.
		*/
		void releaseButton(const Button& p_button);

		/**
		* @brief Edits the wheel position of the mouse.
		*
		* @param p_delta The change in position to apply to the mouse wheel.
		*/
		void editWheelPosition(const Vector2Int& p_delta);

		/**
		* @brief Updates the state of the mouse.
		*/
		void update();

	public:
		/**
		* @brief Places the mouse at a given position.
		*
		* @param p_position The position to place the mouse at.
		*/
		void place(const Vector2Int& p_position);

		/**
		* @brief Gets the input state of a given button.
		*
		* @param p_button The button to get the input state for.
		* @return The input state of the button.
		*/
		const InputState& inputStatus(const Button& p_button) const
		{
			return (_buttons[static_cast<size_t>(p_button)]);
		}

		/**
		* @brief Gets the current position of the mouse.
		* @return The current position of the mouse.
		*/
		const Vector2Int& position() const { return (_position); }

		/**
		* @brief Gets the delta position of the mouse.
		* @return The delta position of the mouse.
		*/
		const Vector2Int& deltaPosition() const { return (_deltaPosition); }

		/**
		* @brief Gets the position of the mouse wheel.
		* @return The position of the mouse wheel.
		*/
		const Vector2Int& wheel() const { return (_wheel); }
	};

	/**
	* @brief Converts a Mouse::Button value to a wstring.
	* @param p_button The Mouse::Button value to convert.
	* @return The converted Mouse::Button value.
	*/
	std::wstring to_wstring(const Mouse::Button& p_button);
}
