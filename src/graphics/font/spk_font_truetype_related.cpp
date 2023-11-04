#include "graphics/spk_font.hpp"

#define STB_TRUETYPE_IMPLEMENTATION
#include "external_libraries/stb_truetype.h"

namespace spk
{
	size_t Font::Configuration::_countNbChar(const std::vector<uint8_t> &p_fontData)
	{
		stbtt_fontinfo fontInfo;
		stbtt_InitFont(&fontInfo, p_fontData.data(), 0);

		int nbGlyph = 0;
		for (wchar_t i = L' '; i < 0xFFFF; i++)
		{
			if (stbtt_FindGlyphIndex(&fontInfo, i) != 0)
				nbGlyph++;
		}

		return (nbGlyph);
	}

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
		errorCode = stbtt_PackFontRange(&context, p_fontData.data(), 0, static_cast<float>(p_key.fontSize), L' ', p_fontConfiguration.nbGlyph(), charInformation);
		stbtt_PackEnd(&context);
		return (errorCode != 0);
	}

	void _computeCharGlyphData(const wchar_t& p_char, spk::Font::Atlas::GlyphData &p_data, const stbtt_packedchar *p_charInformation, const spk::Vector2Int& p_atlasSize, const spk::Vector2& outlineOffset)
	{
		stbtt_aligned_quad quad;
		spk::Vector2 quadStep;
		stbtt_GetPackedQuad(p_charInformation, p_atlasSize.x, p_atlasSize.y, p_char, &quadStep.x, &quadStep.y, &quad, 1);

		p_data.uvs[0] = {quad.s0 + outlineOffset.x * -1, quad.t0 + outlineOffset.y * -1};
		p_data.uvs[1] = {quad.s1 + outlineOffset.x * +1, quad.t0 + outlineOffset.y * -1};
		p_data.uvs[2] = {quad.s0 + outlineOffset.x * -1, quad.t1 + outlineOffset.y * +1};
		p_data.uvs[3] = {quad.s1 + outlineOffset.x * +1, quad.t1 + outlineOffset.y * +1};

		p_data.position[0] = spk::Vector2Int(quad.x0 * p_atlasSize.x, quad.y0 * p_atlasSize.y);
		p_data.position[1] = spk::Vector2Int(quad.x1 * p_atlasSize.x, quad.y0 * p_atlasSize.y);
		p_data.position[2] = spk::Vector2Int(quad.x0 * p_atlasSize.x, quad.y1 * p_atlasSize.y);
		p_data.position[3] = spk::Vector2Int(quad.x1 * p_atlasSize.x, quad.y1 * p_atlasSize.y);

		p_data.step = quadStep * p_atlasSize;
	}

	Font::Atlas::BuildData Font::Atlas::_computeBuildData(const std::vector<uint8_t> &p_fontData, const Configuration &p_fontConfiguration, const Key &p_key)
	{
		BuildData buildData;

		stbtt_packedchar *charInformation = new stbtt_packedchar[p_fontConfiguration.nbGlyph()];

		while (_executePackingOperation(p_fontData,p_fontConfiguration,p_key, buildData.buffer, buildData.size, charInformation) == false)
		{
			buildData.size *= spk::Vector2Int(2, 2);
		}

		spk::Vector2 outlineOffset = (spk::Vector2(1.0f, 1.0f) / static_cast<spk::Vector2>(buildData.size)) * spk::Vector2(p_key.outlineSize, p_key.outlineSize);

		for (wchar_t i = 0; i < p_fontConfiguration.nbGlyph(); i++)
		{
			_computeCharGlyphData(i, _glyphDatas[i], charInformation, buildData.size, outlineOffset);
		}

		return (buildData);
	}
}