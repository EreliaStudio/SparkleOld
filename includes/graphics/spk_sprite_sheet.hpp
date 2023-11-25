#pragma once

#include "graphics/spk_image.hpp"

namespace spk
{
	class SpriteSheet : public Image
	{
	private:
		spk::Vector2UInt _size;
		spk::Vector2 _unit;
		std::vector<spk::Vector2> _sprites;

	public:
		SpriteSheet(const std::filesystem::path& p_path, const spk::Vector2UInt& p_spriteSize);

		const spk::Vector2UInt& size() const;
		const spk::Vector2& unit() const;
		const std::vector<spk::Vector2>& sprites() const;
		size_t spriteID(const spk::Vector2UInt& p_spriteCoord);

		spk::Vector2 sprite(const spk::Vector2UInt& p_spriteCoord);
		spk::Vector2 sprite(const size_t& p_spriteID);
	};
}
