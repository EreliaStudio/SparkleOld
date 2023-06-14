#pragma once

#include "iostream/spk_iostream.hpp"

namespace spk
{
	enum class InputState
	{
		Up,
		Pressed,
		Down,
		Released
	};

	std::wstring to_wstring(const InputState &p_status);
}