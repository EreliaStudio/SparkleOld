#pragma once

#include <string>
#include "math/spk_vector2.hpp"

namespace spk::GraphicalAPI
{
	class AbstractSurface
	{
	private:
		spk::Vector2UInt _size;

		virtual void _onResize() = 0;

	public:
		AbstractSurface(const spk::Vector2UInt& p_size) :
			_size(p_size)
		{

		}
		
		void resize(const spk::Vector2UInt& p_size)
		{
			_size = p_size;
			_onResize();
		}

		virtual void clear() = 0;
		virtual void render() = 0;

	};
}