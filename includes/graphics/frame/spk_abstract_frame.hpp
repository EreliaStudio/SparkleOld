#pragma once

#include <string>
#include "math/spk_vector2.hpp"

namespace spk
{
	class AbstractFrame
	{
	protected:
		spk::Vector2UInt _size;

		virtual void _onResize() = 0;
		virtual void _onSetSize() = 0;

	public:
		AbstractFrame(const spk::Vector2UInt& p_size) :
			_size(p_size)
		{

		}

		AbstractFrame(const AbstractFrame&) = delete;
		AbstractFrame& operator=(const AbstractFrame&) = delete;

		AbstractFrame(AbstractFrame&&) = default;
		AbstractFrame& operator=(AbstractFrame&&) = default;

		virtual void clear() = 0;
		virtual void render() = 0;

		void resize(const spk::Vector2UInt& p_size)
		{
			_size = p_size;
			_onResize();
		}

		void setSize(const spk::Vector2UInt& p_size)
		{
			_size = p_size;
			_onSetSize();
		}

		const Vector2UInt &size() const
        {
            return (_size);
        }
	};
}