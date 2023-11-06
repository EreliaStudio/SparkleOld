#include "graphics/spk_font.hpp"
#include <functional>

namespace spk
{
	/**
	 * @brief Places a pixel using a pixelized outline method.
	 *
	 * This function modifies the atlas data to apply a pixelized outline 
	 * around characters. It updates pixel values based on their distance 
	 * from the base pixel and their alignment with the original pixel of
	 * the font, creating a blocky appearance characteristic of pixelized
	 * fonts.
	 *
	 * @param p_atlasData The atlas data where pixels are stored.
	 * @param p_atlasSize The dimensions of the atlas.
	 * @param p_pixelOrigin The origin of the pixel (usually its top-left corner).
	 * @param p_x The x-coordinate of the current pixel.
	 * @param p_y The y-coordinate of the current pixel.
	 * @param p_key Font key containing information like outline size, etc.
	 */
	void _placePixelPixelized(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_pixelOrigin, const int &p_x, const int &p_y, const spk::Font::Key &p_key)
	{
		size_t baseIndex = p_x + p_y * p_atlasSize.x;

		static const spk::Vector2Int positionDelta[4] = {
			spk::Vector2Int(1, 0),
			spk::Vector2Int(0, 1),
			spk::Vector2Int(-1, 0),
			spk::Vector2Int(0, -1)};

		for (size_t i = 0; i < 4; i++)
		{
			for (uint8_t count = 0; count < p_key.outlineSize; count++)
			{
				int currentX = p_x + positionDelta[i].x * count;
				int currentY = p_y + positionDelta[i].y * count;

				size_t index = (currentX) + (currentY)*p_atlasSize.x;
				if (p_atlasData[index] != Font::CHAR_PIXEL)
				{
					if (p_atlasData[index] < count)
						p_atlasData[index] = count;
				}
			}
		}
	}
}