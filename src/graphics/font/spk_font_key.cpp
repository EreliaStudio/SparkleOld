#include "graphics/spk_font.hpp"

namespace spk
{
	Font::Key::Key(const size_t &p_fontSize, const size_t &p_outlineSize, const OutlineType &p_outlineType) :
		fontSize(p_fontSize),
		outlineSize(p_outlineSize),
		outlineType(p_outlineType),
		inverseOutlineSize(Font::CHAR_PIXEL - p_outlineSize),
		outlineSizeSquared(p_outlineSize * p_outlineSize)
	{
		
	}

	bool Font::Key::operator<(const Key &p_other) const
	{
		if (fontSize < p_other.fontSize)
			return (true);
		if (fontSize == p_other.fontSize && outlineSize < p_other.outlineSize)
			return (true);
		if (fontSize == p_other.fontSize && outlineSize < p_other.outlineSize && static_cast<int>(outlineType) < static_cast<int>(p_other.outlineType))
			return (true);
		return (false);
	}

	void Font::Key::computeCircle(const spk::Vector2Int& p_atlasSize)
	{
		for (int i = -static_cast<int>(outlineSize); i <= static_cast<int>(outlineSize); i++)
		{
			for (int j = -static_cast<int>(outlineSize); j <= static_cast<int>(outlineSize); j++)
			{
				int distance = static_cast<int>(std::ceil(spk::Vector2Int(i, j).distanceSquared(spk::Vector2Int(0, 0))));
			
				if (distance <= static_cast<int>(outlineSizeSquared))
				{
					circleIndexes.push_back(i + j * p_atlasSize.x);
				}
			}	
		}
	}
}