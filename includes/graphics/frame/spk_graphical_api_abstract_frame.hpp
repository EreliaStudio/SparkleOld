#pragma once

#include <string>
#include "math/spk_vector2.hpp"

namespace spk::GraphicalAPI
{
	class AbstractFrame
	{
	private:
		spk::Vector2UInt _size;

		virtual void _onResize() = 0;

	public:
		AbstractFrame(const spk::Vector2UInt& p_size) :
			_size(p_size)
		{

		}

		virtual void clear() = 0;
		virtual void render() = 0;

		void resize(const spk::Vector2UInt& p_size)
		{
			_size = p_size;
			_onResize();
		}

		const Vector2Int &size() const
        {
            return (_size);
        }
	};
}