#include "graphics/spk_font.hpp"

#define STB_TRUETYPE_IMPLEMENTATION
#include "external_libraries/stb_truetype.h"

namespace spk
{
	size_t Font::Configuration::_countNbChar(const std::vector<uint8_t> &p_fontData)
	{
		size_t result = 0;
		int numFonts = stbtt_GetNumberOfFonts(p_fontData.data());

		if (numFonts == 0)
		{
			throw std::runtime_error("No fonts found in the provided font data.");
		}

		for (int fontIndex = 0; fontIndex < numFonts; ++fontIndex)
		{
			stbtt_fontinfo fontInfo;

			if (!stbtt_InitFont(&fontInfo, p_fontData.data(), stbtt_GetFontOffsetForIndex(p_fontData.data(), fontIndex)))
			{
				spk::throwException(L"Can't initiate font from file [" + _fileName + L"]");
			}

			result += fontInfo.numGlyphs;
		}

		return (result);
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
		int startingPackError = stbtt_PackBegin(&context, atlasData.data(), atlasSize.x, atlasSize.y, 0, 1, nullptr);
		if (startingPackError == 0)
			spk::throwException(L"Failed to start the packing process for font [" + p_fontConfiguration.fileName() + L"] with error [" + std::to_wstring(startingPackError) + L"]");
		context.padding = p_key.outlineSize * 2;

		stbtt_PackSetOversampling(&context, 1, 1);
		int packingFontRange = stbtt_PackFontRange(&context, p_fontData.data(), 0, static_cast<float>(p_key.fontSize), 0, p_fontConfiguration.nbGlyph(), charInformation);
		if (packingFontRange == 0)
		{
			stbtt_PackEnd(&context);
			return (false);
		}
		else
		{
			stbtt_PackEnd(&context);
			return (true);
		}
	}

	Font::Atlas::Atlas(const std::vector<uint8_t> &p_fontData, const Configuration &p_fontConfiguration, const Key &p_key)
	{
		std::vector<uint8_t> atlasData;
		spk::Vector2Int atlasSize = spk::Vector2Int(32, 32);
		stbtt_packedchar *charInformation = new stbtt_packedchar[p_fontConfiguration.nbGlyph()];

		while (_executePackingOperation(p_fontData,p_fontConfiguration,p_key, atlasData, atlasSize, charInformation) == false)
		{
			atlasSize *= spk::Vector2Int(2, 2);
		}

		_glyphDatas.resize(p_fontConfiguration.nbGlyph());

		spk::Vector2 outlineOffset = (spk::Vector2(1.0f, 1.0f) / static_cast<spk::Vector2>(atlasSize)) * spk::Vector2(p_key.outlineSize, p_key.outlineSize);

		for (wchar_t i = L' '; i < p_fontConfiguration.nbGlyph(); i++)
		{
			GlyphData &data = _glyphDatas[i];

			stbtt_aligned_quad quad;
			spk::Vector2 quadStep;
			stbtt_GetPackedQuad(charInformation, atlasSize.x, atlasSize.y, i, &quadStep.x, &quadStep.y, &quad, 1);

			data.uvs[0] = {quad.s0 + outlineOffset.x * -1, quad.t0 + outlineOffset.y * -1};
			data.uvs[1] = {quad.s1 + outlineOffset.x * +1, quad.t0 + outlineOffset.y * -1};
			data.uvs[2] = {quad.s0 + outlineOffset.x * -1, quad.t1 + outlineOffset.y * +1};
			data.uvs[3] = {quad.s1 + outlineOffset.x * +1, quad.t1 + outlineOffset.y * +1};

			data.position[0] = spk::Vector2Int(quad.x0 * atlasSize.x, quad.y0 * atlasSize.y);
			data.position[1] = spk::Vector2Int(quad.x1 * atlasSize.x, quad.y0 * atlasSize.y);
			data.position[2] = spk::Vector2Int(quad.x0 * atlasSize.x, quad.y1 * atlasSize.y);
			data.position[3] = spk::Vector2Int(quad.x1 * atlasSize.x, quad.y1 * atlasSize.y);

			data.step = quadStep * atlasSize;
		}

		_applyOutline(atlasData, atlasSize, p_key);

		_texture.uploadToGPU(atlasData.data(), atlasSize,
								Texture::Format::R, Texture::Filtering::Nearest,
								Texture::Wrap::Repeat, Texture::Mipmap::Disable);
	}
}