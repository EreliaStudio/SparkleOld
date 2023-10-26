#include "graphics/spk_area.hpp"

namespace spk
{
	Area::Area()  :
		_anchor(0, 0),
		_size(0, 0)
	{
	}

    Area::Area(const spk::Vector2Int& p_anchor, const spk::Vector2UInt& p_size)  :
		_anchor(p_anchor),
		_size(p_size)
    {

    }

	void Area::setGeometry(const spk::Vector2Int& p_anchor, const spk::Vector2UInt& p_size)
	{
		_anchor = p_anchor;
		_size = p_size;
	}
	
	void Area::setAnchor(const spk::Vector2Int& p_anchor)
	{
		_anchor = p_anchor;
	}

	void Area::move(const spk::Vector2Int& p_delta)
	{
		_anchor += p_delta;
	}
		
	void Area::setSize(const spk::Vector2UInt& p_size)
	{
		_size = p_size;
	}
}