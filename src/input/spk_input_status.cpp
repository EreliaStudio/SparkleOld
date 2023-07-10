#include "input/spk_input_status.hpp"

namespace spk
{
	std::wstring to_wstring(const InputState &p_status)
	{
		switch (p_status)
		{
		case InputState::Up:
			return (L"Up");
		case InputState::Pressed:
			return (L"Pressed");
		case InputState::Down:
			return (L"Down");
		case InputState::Released:
			return (L"Released");
		}
		return (L"Unknown");
	}
}