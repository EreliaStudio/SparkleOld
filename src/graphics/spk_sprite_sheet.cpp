#include "graphics/spk_sprite_sheet.hpp"

namespace spk
{
	SpriteSheet::SpriteSheet(const std::filesystem::path& p_path, const spk::Vector2UInt& p_spriteSize) :
		spk::Image(p_path)
	{
		_size = p_spriteSize;
		_unit = Vector2(1.0f, 1.0f) / _size;

		if (p_spriteSize == Vector2UInt(1, 1))
			_sprites.push_back(Vector2(0, 0));
		else
		{
			for (size_t y = 0; y < _size.y; y++)
			{
				for (size_t x = 0; x < _size.x; x++)
				{
					_sprites.push_back(Vector2(x, y) * _unit);
				}
			}
		}
	}

	const spk::Vector2UInt& SpriteSheet::size() const
	{
		return (_size);
	}

	const spk::Vector2& SpriteSheet::unit() const
	{
		return (_unit);
	}

	const std::vector<spk::Vector2>& SpriteSheet::sprites() const
	{
		return (_sprites);
	}

	size_t SpriteSheet::spriteID(const spk::Vector2UInt& p_spriteCoord)
	{	
		return ((_size.x * p_spriteCoord.y) + p_spriteCoord.x);
	}

	spk::Vector2 SpriteSheet::sprite(const spk::Vector2UInt& p_spriteCoord)
	{
		return (sprite(spriteID(p_spriteCoord)));
	}
	
	spk::Vector2 SpriteSheet::sprite(const size_t& p_spriteID)
	{
		if (_sprites.size() <= p_spriteID)
			throw std::runtime_error("Segfault : sprite id out of range");
		return (_sprites[p_spriteID]);
	}
}