#include "graphics/spk_font.hpp"
#include <functional>

namespace spk
{
	/**
	 * @brief Places a pixel using the standard method to create a "circle" shape outline.
	 *
	 * This function modifies the atlas data to apply a circular outline around 
	 * characters. The outline shape is based on the indices within the font key. 
	 * For each of these indices, if the corresponding pixel in the atlas data 
	 * is marked as empty, it gets filled with a specific value (0x7F).
	 *
	 * @param p_atlasData The atlas data where pixels are stored.
	 * @param p_atlasSize The dimensions of the atlas.
	 * @param p_pixelOrigin The origin of the pixel.
	 * @param p_x The x-coordinate of the current pixel.
	 * @param p_y The y-coordinate of the current pixel.
	 * @param p_key Font key containing indices for the circular outline.
	 */
	void _placePixelStandard(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_pixelOrigin, const int &p_x, const int &p_y, const spk::Font::Key &p_key)
	{
		size_t baseIndex = p_x + p_y * p_atlasSize.x;

		for (size_t i = 0; i < p_key.circleIndexes.size(); i++)
		{
			int index = baseIndex + p_key.circleIndexes[i];

			if (index >= 0 && index < p_atlasSize.x * p_atlasSize.y)
			{
				if (p_atlasData[index] == Font::EMPTY_PIXEL)
					p_atlasData[index] = 0x7F;
			}
		}
	}
}