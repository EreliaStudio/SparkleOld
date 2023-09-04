#pragma once

#include "math/spk_vector2.hpp"

namespace spk
{
	class AbstractSurface
	{
	protected:
		std::wstring _title;
		spk::Vector2UInt _size;

	public:
		AbstractSurface(const std::wstring& p_title, const spk::Vector2UInt& p_size);

		virtual void clear() = 0;
		virtual void resize(const spk::Vector2UInt& p_size) = 0;
		virtual void render() = 0;

		const spk::Vector2UInt& size() const;
	};
}