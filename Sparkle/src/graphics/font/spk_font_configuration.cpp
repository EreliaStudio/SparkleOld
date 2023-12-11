#include "graphics/spk_font.hpp"

namespace spk
{

	Font::Configuration::Configuration() : _fileName(L"Uninitialized")
	{

	}

	Font::Configuration::Configuration(const std::wstring &p_fileName, const std::vector<uint8_t> &p_fontData) : _fileName(p_fileName)
	{
		_computeGlyphInformation(p_fontData);
	}

	const std::wstring &Font::Configuration::fileName() const
	{
		return (_fileName);
	}

	const std::vector<wchar_t>& Font::Configuration::validGlyphs() const
	{
		return (_validGlyphs);
	}
}