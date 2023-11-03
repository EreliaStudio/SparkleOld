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
			for (size_t count = 0; count < p_key.outlineSize; count++)
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
				if (p_atlasData[index] > p_atlasData[baseIndex] + 1)
				{
					int distance = static_cast<int>(std::ceil(spk::Vector2Int(index % p_atlasSize.x, index / p_atlasSize.x).distanceSquared(p_pixelOrigin)));

					if (distance <= static_cast<int>(p_key.outlineSizeSquared))
					{
						p_atlasData[index] = std::sqrt(distance);

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

	void spk::Font::Atlas::_applyOutline(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const Key &p_key)
	{
		for (size_t i = 0; i < static_cast<size_t>(p_atlasSize.x * p_atlasSize.y); i++)
		{
			if (p_atlasData[i] != 0xFF)
				p_atlasData[i] = 0xFF;
			else
				p_atlasData[i] = 0x00;
		}

		std::function<void(std::vector<uint8_t> &, const spk::Vector2Int &, const spk::Vector2Int&, const int &, const int &, const Key &)> placePixelFunction;
		switch (p_key.outlineType)
		{
		case OutlineType::Pixelized:
			placePixelFunction = _placePixelPixelized;
			break;
		case OutlineType::SharpEdge:
			placePixelFunction = _placePixelSharpEdge;
			break;
		case OutlineType::Standard:
			placePixelFunction = _placePixelStandard;
			break;
		case OutlineType::Manhattan:
			placePixelFunction = _placePixelManhattan;
			break;
		}

		for (int index = 0; index < p_atlasSize.x * p_atlasSize.y; index++)
		{
			// only call placePixelFunction if the pixel is 0x00 and at least one neighbor is not 0x00
			if (p_atlasData[index] == 0x00
				&& (p_atlasData[index + 1] != 0x00
				|| p_atlasData[index - 1] != 0x00
				|| p_atlasData[index + p_atlasSize.y] != 0x00
				|| p_atlasData[index - p_atlasSize.y] != 0x00)
			)
			{
				placePixelFunction(
					p_atlasData,
					p_atlasSize,
					spk::Vector2Int(index % p_atlasSize.x, index / p_atlasSize.x),
					index % p_atlasSize.x,
					index / p_atlasSize.x,
					p_key
				);
			}
		}

		for (size_t i = 0; i < static_cast<size_t>(p_atlasSize.x * p_atlasSize.y); i++)
		{
			if (p_atlasData[i] == 0xFF)
				p_atlasData[i] = 0x00;
			else if (p_atlasData[i] == 0x00)
				p_atlasData[i] = 0xFF;
			else
			{
				p_atlasData[i] = static_cast<float>(p_atlasData[i] * 256) / static_cast<float>(p_key.outlineSize);
			}
		}
	}
}