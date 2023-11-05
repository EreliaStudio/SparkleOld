#include "graphics/spk_font.hpp"
#include <functional>

namespace spk
{
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