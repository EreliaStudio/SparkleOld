#include "graphics/graphical_device/spk_abstract_graphical_device.hpp"

namespace spk
{
	std::wstring to_wstring(const spk::AbstractGraphicalDevice::Storage::Description::Buffer::Type& p_input)
	{
		switch (p_input)
		{
			case (spk::AbstractGraphicalDevice::Storage::Description::Buffer::Type::Float):
				return (L"FLOAT");
			case (spk::AbstractGraphicalDevice::Storage::Description::Buffer::Type::UInt):
				return (L"UNSIGNED_INT");
			case (spk::AbstractGraphicalDevice::Storage::Description::Buffer::Type::Int):
				return (L"INT");
		}
		return (L"GL_NONE");
	}
}