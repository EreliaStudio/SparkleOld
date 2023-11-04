#include "graphics/spk_font.hpp"
#include <functional>

namespace spk
{
	size_t nbPlacePixel = 0;

	void _placePixelPixelized(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_pixelOrigin, const int &p_x, const int &p_y, const spk::Font::Key &p_key);
	void _placePixelSharpEdge(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_pixelOrigin, const int &p_x, const int &p_y, const spk::Font::Key &p_key);
	void _placePixelStandard(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_pixelOrigin, const int &p_x, const int &p_y, const spk::Font::Key &p_key);
	void _placePixelManhattan(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_pixelOrigin, const int &p_x, const int &p_y, const spk::Font::Key &p_key);

	using PlacePixelFunction = std::function<void(std::vector<uint8_t> &, const spk::Vector2Int &, const spk::Vector2Int&, const spk::Font::Key &)>;

	PlacePixelFunction _requestPlacePixelFunction(const spk::Font::OutlineType& p_outlineType)
	{
		PlacePixelFunction result = nullptr;
		switch (p_outlineType)
		{
		case spk::Font::OutlineType::Pixelized:
			result = [](std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_originPixel, const spk::Font::Key &p_key){
				_placePixelPixelized(p_atlasData, p_atlasSize, p_originPixel, p_originPixel.x, p_originPixel.y, p_key);
			};
			break;
		case spk::Font::OutlineType::SharpEdge:
			result = [](std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_originPixel, const spk::Font::Key &p_key){
				_placePixelSharpEdge(p_atlasData, p_atlasSize, p_originPixel, p_originPixel.x, p_originPixel.y, p_key);
			};
			break;
		case spk::Font::OutlineType::Standard:
			result = [](std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_originPixel, const spk::Font::Key &p_key){
				_placePixelStandard(p_atlasData, p_atlasSize, p_originPixel, p_originPixel.x, p_originPixel.y, p_key);
			};
			break;
		case spk::Font::OutlineType::Manhattan:
			result = [](std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_originPixel, const spk::Font::Key &p_key){
				_placePixelManhattan(p_atlasData, p_atlasSize, p_originPixel, p_originPixel.x, p_originPixel.y, p_key);
			};
			break;
		}
		return (result);
	}

	bool _isPixelOnBorder(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const size_t& p_index)
	{
		return (p_atlasData[p_index] == 0x00 && (p_atlasData[p_index + 1] != 0x00 || p_atlasData[p_index - 1] != 0x00 || p_atlasData[p_index + p_atlasSize.y] != 0x00 || p_atlasData[p_index - p_atlasSize.y] != 0x00));
	}

	void _revertAtlasData(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize)
	{
		size_t maxIndex = static_cast<size_t>(p_atlasSize.x * p_atlasSize.y);
		for (size_t i = 0; i < maxIndex; i++)
		{
			if (p_atlasData[i] == 0xFF)
				p_atlasData[i] = 0x00;
			else if (p_atlasData[i] == 0x00)
				p_atlasData[i] = 0xFF;
			else
			{
				p_atlasData[i] = 0x7F;
			}
		}
	}

	void _applyOutline(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Font::Key &p_key)
	{
		_revertAtlasData(p_atlasData, p_atlasSize);

		PlacePixelFunction placePixelFunction = _requestPlacePixelFunction(p_key.outlineType);
			
		if (placePixelFunction != nullptr)
		{
			size_t maxIndex = static_cast<size_t>(p_atlasSize.x * p_atlasSize.y);
			for (size_t index = 0; index < maxIndex; index++)
			{
				if (_isPixelOnBorder(p_atlasData, p_atlasSize, index) == true)
				{
					placePixelFunction(
						p_atlasData,
						p_atlasSize,
						spk::Vector2Int(index % p_atlasSize.x, index / p_atlasSize.x),
						p_key
					);
				}
			}
			
		}

		_revertAtlasData(p_atlasData, p_atlasSize);
	}
}