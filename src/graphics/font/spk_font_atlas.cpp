#include "graphics/spk_font.hpp"

namespace spk
{
	void _applyOutline(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Font::Key &p_key);

	Font::Atlas::Atlas()
	{
	}

	/**
	 * @brief Normalizes the atlas data.
	 *
	 * This function iterates through all pixels in the atlas and normalizes 
	 * their values. Any pixel not representing a character (`Font::CHAR_PIXEL`) 
	 * is set to be empty (`Font::EMPTY_PIXEL`). Pixels representing characters 
	 * remain unchanged. This ensures the atlas data contains only distinct 
	 * values corresponding to character pixels and empty pixels.
	 *
	 * @param p_atlasData The atlas data where pixels are stored.
	 * @param p_atlasSize The dimensions of the atlas.
	 */
	void _normalizeAtlasData(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize)
	{
		size_t maxIndex = static_cast<size_t>(p_atlasSize.x * p_atlasSize.y);
		for (size_t i = 0; i < maxIndex; i++)
		{
			if (p_atlasData[i] != Font::CHAR_PIXEL)
				p_atlasData[i] = Font::EMPTY_PIXEL;
			else
				p_atlasData[i] = Font::CHAR_PIXEL;
		}
	}
	
	Font::Atlas::Atlas(const std::vector<uint8_t> &p_fontData, const Configuration &p_fontConfiguration, const Key &p_key) :
		_glyphDatas(p_fontConfiguration.validGlyphs().back() + 1)
	{
		Font::Atlas::BuildData buildData = _computeBuildData(p_fontData, p_fontConfiguration, p_key);

		_normalizeAtlasData(buildData.buffer, buildData.size);

		if (p_key.outlineSize != 0)
		{		
			if (p_key.outlineType == Font::OutlineType::Standard)
			{
				(const_cast<Key*>(&p_key))->computeCircle(buildData.size);
			}
		
			_applyOutline(buildData.buffer, buildData.size, p_key);
		}

		_texture.uploadToGPU(
			buildData.buffer.data(), buildData.size,
			Texture::Format::R, Texture::Filtering::Nearest,
			Texture::Wrap::Repeat, Texture::Mipmap::Disable);
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