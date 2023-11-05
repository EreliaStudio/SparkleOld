#include "graphics/spk_font.hpp"

namespace spk
{
	void _applyOutline(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Font::Key &p_key);

	Font::Atlas::Atlas()
	{
	}

	void _normalizeAtlasData(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize)
	{
		size_t maxIndex = static_cast<size_t>(p_atlasSize.x * p_atlasSize.y);
		for (size_t i = 0; i < maxIndex; i++)
		{
			if (p_atlasData[i] != 0xFF)
				p_atlasData[i] = 0x00;
			else
				p_atlasData[i] = 0xFF;
		}
	}
	
	Font::Atlas::Atlas(const std::vector<uint8_t> &p_fontData, const Configuration &p_fontConfiguration, const Key &p_key) :
		_glyphDatas(p_fontConfiguration.validGlyphs().back() + 1)
	{
		Font::Atlas::BuildData buildData = _computeBuildData(p_fontData, p_fontConfiguration, p_key);

		_normalizeAtlasData(buildData.buffer, buildData.size);

		if (p_key.outlineSize != 0)
			_applyOutline(buildData.buffer, buildData.size, p_key);

		_texture.uploadToGPU(
			buildData.buffer.data(), buildData.size,
			Texture::Format::R, Texture::Filtering::Nearest,
			Texture::Wrap::Repeat, Texture::Mipmap::Disable);
	}

	const Font::Atlas::GlyphData &Font::Atlas::glyph(const wchar_t &p_char) const
	{
		if (_glyphDatas.size() < static_cast<size_t>(p_char - L' '))
			spk::throwException(L"Char [" + std::wstring(1, p_char) + L"](dec : " + std::to_wstring(static_cast<size_t>(p_char)) + L") cannot be rendered : it doesn't exist in Font [" + _fontConfiguration.fileName() + L"]");
		return (_glyphDatas[static_cast<size_t>(p_char - L' ')]);
	}

	const Texture &Font::Atlas::texture() const
	{
		return (_texture);
	}
}