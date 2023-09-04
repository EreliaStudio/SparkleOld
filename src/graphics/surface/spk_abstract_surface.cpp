#include "graphics/surface/spk_abstract_surface.hpp"

namespace spk
{
	AbstractSurface::AbstractSurface(const std::wstring& p_title, const spk::Vector2UInt& p_size) :
		_title(p_title),
		_size(p_size)
	{}

	const spk::Vector2UInt& AbstractSurface::size() const
	{
		return (_size);
	}
}