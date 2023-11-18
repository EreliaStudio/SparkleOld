#include "graphics/frame/spk_abstract_frame.hpp"

namespace spk
{
	AbstractFrame::AbstractFrame(const spk::Vector2UInt& p_size) :
		_size(p_size)
	{

	}

	void AbstractFrame::resize(const spk::Vector2UInt& p_size)
	{
		_size = p_size;
		_onResize();
	}

	void AbstractFrame::place(const spk::Vector2Int& p_anchor)
	{
		_anchor = p_anchor;
	}

	void AbstractFrame::setSize(const spk::Vector2UInt& p_size)
	{
		_size = p_size;
		_onSetSize();
	}

	const Vector2UInt &AbstractFrame::size() const
	{
		return (_size);
	}

	const Vector2Int &AbstractFrame::anchor() const
	{
		return (_anchor);
	}
}