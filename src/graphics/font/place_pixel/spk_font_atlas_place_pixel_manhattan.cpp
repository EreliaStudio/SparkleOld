#include "graphics/spk_font.hpp"
#include <functional>

namespace spk
{
	void _placePixelManhattan(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_pixelOrigin, const int &p_x, const int &p_y, const spk::Font::Key &p_key)
	{
		size_t baseIndex = p_x + p_y * p_atlasSize.x;

		static const spk::Vector2Int positionDelta[4] = {
			spk::Vector2Int(1, 0),
			spk::Vector2Int(0, 1),
			spk::Vector2Int(-1, 0),
			spk::Vector2Int(0, -1)};

		for (size_t i = 0; i < 4; i++)
		{
			int currentX = p_x + positionDelta[i].x;
			int currentY = p_y + positionDelta[i].y;

			if (currentX >= 0 && currentY >= 0 &&
				currentX < p_atlasSize.x && currentY < p_atlasSize.y)
			{
				size_t index = (currentX) + (currentY)*p_atlasSize.x;
				if (p_atlasData[index] > (p_atlasData[baseIndex] + 1))
				{
					p_atlasData[index] = p_atlasData[baseIndex] + 1;

					if (p_atlasData[index] < p_key.outlineSize)
						_placePixelManhattan(p_atlasData, p_atlasSize, p_pixelOrigin, currentX, currentY, p_key);
				}
			}
		}
	}
}