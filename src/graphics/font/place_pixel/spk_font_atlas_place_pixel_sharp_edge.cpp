#include "graphics/spk_font.hpp"
#include <functional>

namespace spk
{
	/**
	 * @brief Places a pixel using a sharp edge outline method.
	 *
	 * This function modifies the atlas data to apply a sharp edged outline 
	 * around characters. The outline is formed by filling the entire area around 
	 * the origin pixel, covering every pixel in a square of dimensions 
	 * `outlineSize * 2` by `outlineSize * 2` centered around the origin.
	 *
	 * Pixels in this area will be modified based on their distance from the 
	 * origin pixel to give the appearance of a sharp edge. The outline's depth 
	 * and spread are determined by the outline size specified in the font key.
	 *
	 * @note This approach is similar to the Pixelized outline type but creates 
	 * a more uniformly sharp appearance.
	 *
	 * @param p_atlasData The atlas data where pixels are stored.
	 * @param p_atlasSize The dimensions of the atlas.
	 * @param p_pixelOrigin The origin of the pixel (usually its top-left corner).
	 * @param p_x The x-coordinate of the current pixel.
	 * @param p_y The y-coordinate of the current pixel.
	 * @param p_key Font key containing information like outline size, etc.
	 */
	void _placePixelSharpEdge(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_pixelOrigin, const int &p_x, const int &p_y, const spk::Font::Key &p_key)
	{
		for (int i = -static_cast<int>(p_key.outlineSize); i < static_cast<int>(p_key.outlineSize); i++)
		{
			for (int j = -static_cast<int>(p_key.outlineSize); j < static_cast<int>(p_key.outlineSize); j++)
			{
				int currentX = p_x + i;
				int currentY = p_y + j;

				if (currentX >= 0 && currentY >= 0 &&
					currentX < p_atlasSize.x && currentY < p_atlasSize.y)
				{
					size_t index = (currentX) + (currentY)*p_atlasSize.x;

					if (p_atlasData.at(index) != Font::CHAR_PIXEL)
					{
						int maxValue = std::max(std::abs(i), std::abs(j));
						if (p_atlasData.at(index) < maxValue)
						{
							p_atlasData[index] = maxValue;
						}
					}
				}
			}
		}
	}
}