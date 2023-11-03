#include "graphics/spk_font.hpp"

namespace spk
{
	Font::Atlas::Atlas()
	{
	}

	const Font::Atlas::GlyphData &Font::Atlas::glyph(const wchar_t &p_char) const
	{
		if (_glyphDatas.size() < static_cast<size_t>(p_char))
			spk::throwException(L"Char [" + std::wstring(1, p_char) + L"](dec : " + std::to_wstring(static_cast<size_t>(p_char)) + L") cannot be rendered : it doesn't exist in Font [" + _fontConfiguration.fileName() + L"]");
		return (_glyphDatas[static_cast<size_t>(p_char)]);
	}

	const Texture &Font::Atlas::texture() const
	{
		return (_texture);
	}
}