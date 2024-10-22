#include "graphics/spk_font.hpp"

#define STB_TRUETYPE_IMPLEMENTATION
#include "external_libraries/stb_truetype.h"

namespace spk
{
	/**
	 * @brief Retrieves the codepoints present in the font.
	 *
	 * Iterates through the range of valid wchar_t values and identifies 
	 * which ones have valid glyphs in the given font.
	 *
	 * @param p_fontInfo Pointer to the font information structure.
	 * @return A vector containing the valid codepoints for the font.
	 */
	std::vector<wchar_t> getCodepointsInFont(const stbtt_fontinfo* p_fontInfo)
	{
		std::vector<wchar_t> result;

		wchar_t i = 32;
		while (i < static_cast<wchar_t>(0xFFFF))
		{
			if (stbtt_FindGlyphIndex(p_fontInfo, i) != 0)
				result.push_back(i);

			i++;
		}

		return result;
	}

	void Font::Configuration::_computeGlyphInformation(const std::vector<uint8_t> &p_fontData)
	{
		stbtt_fontinfo fontInfo;
		stbtt_InitFont(&fontInfo, p_fontData.data(), 0);

		_validGlyphs = getCodepointsInFont(&fontInfo);
	}

	/**
	 * @brief Executes the packing operation for the font.
	 *
	 * Uses stbtt to pack the glyphs of a font into an atlas.
	 * 
	 * @param p_fontData The font data vector.
	 * @param p_fontConfiguration The font configuration.
	 * @param p_key The key which defines the font characteristics.
	 * @param atlasData The resulting atlas data after packing.
	 * @param atlasSize The dimensions of the atlas.
	 * @param charInformation The packed character information after packing.
	 * @return True if the packing operation is successful, false otherwise.
	 */
	bool _executePackingOperation(
		const std::vector<uint8_t> &p_fontData,
		const Font::Configuration &p_fontConfiguration,
		const Font::Key &p_key,
		std::vector<uint8_t>& atlasData,
		const spk::Vector2Int& atlasSize,
		stbtt_packedchar *charInformation)
	{
		atlasData.resize(atlasSize.x * atlasSize.y);

		stbtt_pack_context context;
		int errorCode;

		errorCode = stbtt_PackBegin(&context, atlasData.data(), atlasSize.x, atlasSize.y, 0, 1, nullptr);
		if (errorCode == 0)
			spk::throwException(L"Failed to start the packing process for font [" + p_fontConfiguration.fileName() + L"] with error [" + std::to_wstring(errorCode) + L"]");
		
		if (p_key.outlineSize != 0)
			context.padding = p_key.outlineSize * 2;

		stbtt_PackSetOversampling(&context, 1, 1);
		errorCode = stbtt_PackFontRange(&context, p_fontData.data(), 0, static_cast<float>(p_key.fontSize), L' ', p_fontConfiguration.validGlyphs().back() + 1, charInformation);
		stbtt_PackEnd(&context);

		return (errorCode != 0);
	}

	/**
	 * @brief Computes glyph data for a character.
	 *
	 * Using the packed character information, this function extracts the 
	 * UVs and positions for the character's glyph, considering an outline offset.
	 * 
	 * @param p_char The character for which the glyph data is needed.
	 * @param p_data The resulting glyph data.
	 * @param p_charInformation The packed character information.
	 * @param p_atlasSize The dimensions of the atlas.
	 * @param outlineOffset The offset for the outline.
	 */
	void _computeCharGlyphData(const wchar_t& p_char, spk::Font::Atlas::GlyphData &p_data, const stbtt_packedchar *p_charInformation, const spk::Vector2Int& p_atlasSize, const spk::Vector2& outlineOffset)
	{
		stbtt_aligned_quad quad;
		spk::Vector2 quadStep;

		stbtt_GetPackedQuad(p_charInformation, p_atlasSize.x, p_atlasSize.y, p_char - L' ', &quadStep.x, &quadStep.y, &quad, 1);

		p_data.uvs[0] = {quad.s0 + outlineOffset.x * -1, quad.t0 + outlineOffset.y * -1};
		p_data.uvs[1] = {quad.s1 + outlineOffset.x * +1, quad.t0 + outlineOffset.y * -1};
		p_data.uvs[2] = {quad.s0 + outlineOffset.x * -1, quad.t1 + outlineOffset.y * +1};
		p_data.uvs[3] = {quad.s1 + outlineOffset.x * +1, quad.t1 + outlineOffset.y * +1};

		p_data.position[0] = spk::Vector2Int(quad.x0, quad.y0);
		p_data.position[1] = spk::Vector2Int(quad.x1, quad.y0);
		p_data.position[2] = spk::Vector2Int(quad.x0, quad.y1);
		p_data.position[3] = spk::Vector2Int(quad.x1, quad.y1);

		p_data.step = quadStep;
	}

	Font::Atlas::BuildData Font::Atlas::_computeBuildData(const std::vector<uint8_t> &p_fontData, const Configuration &p_fontConfiguration, const Key &p_key)
	{
		BuildData buildData;

		static stbtt_packedchar *charInformation = new stbtt_packedchar[0xFFFF];

		while (_executePackingOperation(p_fontData,p_fontConfiguration,p_key, buildData.buffer, buildData.size, charInformation) == false)
		{
			buildData.size *= spk::Vector2Int(2, 2);
		}

		spk::Vector2 outlineOffset = (spk::Vector2(1.0f, 1.0f) / static_cast<spk::Vector2>(buildData.size)) * spk::Vector2(p_key.outlineSize, p_key.outlineSize);

		for (size_t i = 0; i < p_fontConfiguration.validGlyphs().size(); i++)
		{
			_computeCharGlyphData( p_fontConfiguration.validGlyphs()[i], _glyphDatas[p_fontConfiguration.validGlyphs()[i]], charInformation, buildData.size, outlineOffset);
		}

		return (buildData);
	}
}