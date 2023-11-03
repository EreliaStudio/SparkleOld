#include "graphics/spk_font.hpp"

namespace spk
{
	Font::Key::Key(const size_t &p_fontSize, const size_t &p_outlineSize, const OutlineType &p_outlineType) : fontSize(p_fontSize),
																															outlineSize(p_outlineSize),
																															outlineType(p_outlineType),
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
}