#include "graphics/spk_font.hpp"
#include <functional>

namespace spk
{
	size_t nbPlacePixel = 0;

	void _placePixelPixelized(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_pixelOrigin, const int &p_x, const int &p_y, const spk::Font::Key &p_key)
	{
		size_t baseIndex = p_x + p_y * p_atlasSize.x;

		static const spk::Vector2Int positionDelta[4] = {
			spk::Vector2Int(1, 0),
			spk::Vector2Int(0, 1),
			spk::Vector2Int(-1, 0),
			spk::Vector2Int(0, -1)};

		for (size_t i = 0; i < 4; i++)
		{
			for (uint8_t count = 0; count < p_key.outlineSize; count++)
			{
				int currentX = p_x + positionDelta[i].x * count;
				int currentY = p_y + positionDelta[i].y * count;

				size_t index = (currentX) + (currentY)*p_atlasSize.x;
				if (p_atlasData[index] != 0x00)
				{
					if (p_atlasData[index] > count)
						p_atlasData[index] = count;
				}
			}
		}
	}

	void _placePixelSharpEdge(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_pixelOrigin, const int &p_x, const int &p_y, const spk::Font::Key &p_key)
	{
		for (int i = -static_cast<int>(p_key.outlineSize); i < static_cast<int>(p_key.outlineSize); i++)
		{
			for (int j = -static_cast<int>(p_key.outlineSize); j < static_cast<int>(p_key.outlineSize); j++)
			{
				int currentX = p_x + i;
				int currentY = p_y + j;

				if (currentX >= 0 && currentY >= 0 &&
					currentX < p_atlasSize.x && currentY < p_atlasSize.y)
				{
					size_t index = (currentX) + (currentY)*p_atlasSize.x;

					if (p_atlasData.at(index) != 0x00)
					{
						int maxValue = std::max(std::abs(i), std::abs(j));
						if (p_atlasData.at(index) > maxValue)
						{
							p_atlasData[index] = maxValue;
						}
					}
				}
			}
		}
	}

	void _placePixelStandard(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_pixelOrigin, const int &p_x, const int &p_y, const spk::Font::Key &p_key)
	{
		size_t baseIndex = p_x + p_y * p_atlasSize.x;

		static const spk::Vector2Int positionDelta[4] = {
			spk::Vector2Int(1, 0),
			spk::Vector2Int(0, 1),
			spk::Vector2Int(-1, 0),
			spk::Vector2Int(0, -1)};

		for (size_t i = 0; i < 4; i++)
		{
			int currentX = p_x + positionDelta[i].x;
			int currentY = p_y + positionDelta[i].y;

			if (currentX >= 0 && currentY >= 0 &&
				currentX < p_atlasSize.x && currentY < p_atlasSize.y)
			{
				size_t index = (currentX) + (currentY)*p_atlasSize.x;
				if (p_atlasData[index] >= p_atlasData[baseIndex] + 1)
				{
					int distance = static_cast<int>(std::ceil(spk::Vector2Int(index % p_atlasSize.x, index / p_atlasSize.x).distanceSquared(p_pixelOrigin)));
					
					if (distance <= static_cast<int>(p_key.outlineSizeSquared))
					{
						p_atlasData[index] = static_cast<uint8_t>(std::sqrt(distance));

						_placePixelStandard(p_atlasData, p_atlasSize, p_pixelOrigin, currentX, currentY, p_key);
					}
				}
			}
		}
	}

	void _placePixelManhattan(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_pixelOrigin, const int &p_x, const int &p_y, const spk::Font::Key &p_key)
	{
		size_t baseIndex = p_x + p_y * p_atlasSize.x;

		static const spk::Vector2Int positionDelta[4] = {
			spk::Vector2Int(1, 0),
			spk::Vector2Int(0, 1),
			spk::Vector2Int(-1, 0),
			spk::Vector2Int(0, -1)};

		for (size_t i = 0; i < 4; i++)
		{
			int currentX = p_x + positionDelta[i].x;
			int currentY = p_y + positionDelta[i].y;

			if (currentX >= 0 && currentY >= 0 &&
				currentX < p_atlasSize.x && currentY < p_atlasSize.y)
			{
				size_t index = (currentX) + (currentY)*p_atlasSize.x;
				if (p_atlasData[index] > (p_atlasData[baseIndex] + 1))
				{
					p_atlasData[index] = p_atlasData[baseIndex] + 1;

					if (p_atlasData[index] < p_key.outlineSize)
						_placePixelManhattan(p_atlasData, p_atlasSize, p_pixelOrigin, currentX, currentY, p_key);
				}
			}
		}
	}

	void spk::Font::Atlas::_normalizeAtlasData(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize)
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

	void spk::Font::Atlas::_revertAtlasData(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize)
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

	void spk::Font::Atlas::_applyOutline(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const Key &p_key)
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