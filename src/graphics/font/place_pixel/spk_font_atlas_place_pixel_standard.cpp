#include "graphics/spk_font.hpp"
#include <functional>

namespace spk
{
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