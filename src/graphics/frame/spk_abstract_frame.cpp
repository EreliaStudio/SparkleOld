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

	void AbstractFrame::setSize(const spk::Vector2UInt& p_size)
	{
		_size = p_size;
		_onSetSize();
	}

	const Vector2UInt &AbstractFrame::size() const
	{
		return (_size);
	}
}