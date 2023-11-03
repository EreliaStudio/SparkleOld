#include "graphics/spk_font.hpp"

namespace spk
{

	Font::Configuration::Configuration() : _fileName(L"Uninitialized"),
										   _nbGlyph(0)
	{
	}

	Font::Configuration::Configuration(const std::wstring &p_fileName, const std::vector<uint8_t> &p_fontData) : _fileName(p_fileName),
																												 _nbGlyph(_countNbChar(p_fontData))
	{
	}

	const std::wstring &Font::Configuration::fileName() const
	{
		return (_fileName);
	}

	const size_t &Font::Configuration::nbGlyph() const
	{
		return (_nbGlyph);
	}
}