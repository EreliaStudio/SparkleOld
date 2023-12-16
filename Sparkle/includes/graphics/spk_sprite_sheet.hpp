#pragma once

#include "graphics/spk_image.hpp"

namespace spk
{
	/**
	 * @class SpriteSheet
	 * @brief A class representing a sprite sheet, derived from an Image class.
	 * 
	 * This class manages a sprite sheet by storing individual sprites' positions and providing access to them.
	 */
	class SpriteSheet : public Image
	{
	private:
		spk::Vector2UInt _size; ///< Size of each sprite in the sprite sheet.
		spk::Vector2 _unit;     ///< Unit size of the sprite sheet in terms of width and height.
		std::vector<spk::Vector2> _sprites; ///< Vector storing the positions of individual sprites.

	public:
		/**
		 * @brief Constructor for SpriteSheet.
		 * @param p_path Path to the sprite sheet image file.
		 * @param p_spriteSize Size of each sprite within the sprite sheet.
		 */
		SpriteSheet(const std::filesystem::path& p_path, const spk::Vector2UInt& p_spriteSize);

		/**
		 * @brief Gets the size of the sprites in the sprite sheet.
		 * @return Constant reference to spk::Vector2UInt representing the size of sprites.
		 */
		const spk::Vector2UInt& size() const;

		/**
		 * @brief Gets the unit size of the sprite sheet.
		 * @return Constant reference to spk::Vector2 representing the unit size.
		 */
		const spk::Vector2& unit() const;

		/**
		 * @brief Gets the positions of all sprites in the sprite sheet.
		 * @return Constant reference to a vector of spk::Vector2 representing sprite positions.
		 */
		const std::vector<spk::Vector2>& sprites() const;

		/**
		 * @brief Gets the ID of the sprite at the specified coordinates.
		 * @param p_spriteCoord Coordinates of the sprite in the sprite sheet.
		 * @return Size_t representing the ID of the sprite.
		 */
		size_t spriteID(const spk::Vector2UInt& p_spriteCoord);

		/**
		 * @brief Retrieves the position of the sprite at the given coordinates.
		 * @param p_spriteCoord Coordinates of the sprite in the sprite sheet.
		 * @return spk::Vector2 representing the position of the sprite.
		 */
		spk::Vector2 sprite(const spk::Vector2UInt& p_spriteCoord);

		/**
		 * @brief Retrieves the position of the sprite with the given ID.
		 * @param p_spriteID ID of the sprite in the sprite sheet.
		 * @return spk::Vector2 representing the position of the sprite.
		 */
		spk::Vector2 sprite(const size_t& p_spriteID);
	};
}
