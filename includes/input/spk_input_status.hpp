#pragma once

#include "iostream/spk_iostream.hpp"

namespace spk
{
	/**
	 * @enum InputState
	 * @brief The state of an input device like a button or key.
	 * 
	 * This enum represents the possible states of an input device like a button or a key:
	 * Up: The input device is not active.
	 * Pressed: The input device just became active.
	 * Down: The input device is still active.
	 * Released: The input device just became inactive.
	 */
	enum class InputState
	{
		Up,
		Pressed,
		Down,
		Released
	};

	/**
	 * @brief Converts an InputState value to a string.
	 * 
	 * This function returns a string representation of an InputState value.
	 * This can be useful for debugging or logging.
	 *
	 * @param p_status The InputState value to convert.
	 * @return A string representation of the InputState value.
	 */
	std::wstring to_wstring(const InputState &p_status);
}
