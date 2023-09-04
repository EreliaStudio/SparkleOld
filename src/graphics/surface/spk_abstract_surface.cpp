#include "graphics/surface/spk_abstract_surface.hpp"

namespace spk
{
	AbstractSurface::AbstractSurface(const std::wstring& p_title, const spk::Vector2UInt& p_size) :
		_title(p_title),
		_size(p_size)
	{}
	
	AbstractSurface::~AbstractSurface()
	{

	}

	void AbstractSurface::activateViewport(const spk::Viewport* p_viewport)
	{
		_activeViewport = p_viewport;
	}	
	
	void AbstractSurface::activateScissor(const spk::Scissor* p_scissor)
	{
		_activeViewport = p_scissor;
	}

	spk::Vector2 AbstractSurface::convertScreenToGPU(const spk::Vector2Int& p_screenPosition)
	{        
		return (spk::Vector2(
				2.0f * (static_cast<float>(p_screenPosition.x - _activeViewport->anchor().x) / _activeViewport->size().x) - 1.0f,
				2.0f * (static_cast<float>(p_screenPosition.y - _activeViewport->anchor().y) / _activeViewport->size().y) - 1.0f
			));
	}

	const spk::Vector2UInt& AbstractSurface::size() const
	{
		return (_size);
	}

	const Viewport* AbstractSurface::activeViewport() const
	{
		return (_activeViewport);
	}
	
	const Scissor* AbstractSurface::activeScissor() const
	{
		return (_activeScissor);
	}
}