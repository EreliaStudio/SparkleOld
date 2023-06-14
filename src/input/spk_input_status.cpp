#include "input/spk_input_status.hpp"

namespace spk
{
	std::wstring to_wstring(const InputStatus &p_status)
	{
		switch (p_status)
		{
		case InputStatus::Up:
			return (L"Up");
		case InputStatus::Pressed:
			return (L"Pressed");
		case InputStatus::Down:
			return (L"Down");
		case InputStatus::Released:
			return (L"Released");
		}
		return (L"Unknown");
	}
}