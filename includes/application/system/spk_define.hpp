#pragma once

#include "iostream/spk_iostream.hpp"

namespace spk
{
	enum class InputStatus
	{
		Up,
		Pressed,
		Down,
		Released
	};

	std::wstring to_wstring(const InputStatus &p_status);
}