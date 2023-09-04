#pragma once

#include "math/spk_vector2.hpp"

namespace spk
{
	class Area
	{
	private:
		spk::Vector2Int _anchor;
		spk::Vector2UInt _size;

	public:
		Area();

		void setGeometry(const spk::Vector2Int& p_anchor, const spk::Vector2UInt& p_size);
		void setAnchor(const spk::Vector2Int& p_anchor);
		void move(const spk::Vector2Int& p_delta);
		void setSize(const spk::Vector2UInt& p_size);

		constexpr const spk::Vector2Int& anchor() const
		{
			return (_anchor);
		}

		constexpr const spk::Vector2UInt& size() const
		{
			return (_size);
		}
	};

	using Viewport = Area;
	using Scissor = Area;
}