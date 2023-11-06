#include "graphics/spk_font.hpp"
#include <functional>

namespace spk
{
	/**
     * @brief Places a pixel with the Pixelized outline type.
     * @param p_atlasData The atlas data.
     * @param p_atlasSize Size of the atlas.
     * @param p_pixelOrigin The origin of the pixel.
     * @param p_x X-coordinate.
     * @param p_y Y-coordinate.
     * @param p_key Font key information.
     */
	void _placePixelPixelized(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_pixelOrigin, const int &p_x, const int &p_y, const spk::Font::Key &p_key);
	
	/**
     * @brief Places a pixel with the SharpEdge outline type.
     * @param p_atlasData The atlas data.
     * @param p_atlasSize Size of the atlas.
     * @param p_pixelOrigin The origin of the pixel.
     * @param p_x X-coordinate.
     * @param p_y Y-coordinate.
     * @param p_key Font key information.
     */
	void _placePixelSharpEdge(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_pixelOrigin, const int &p_x, const int &p_y, const spk::Font::Key &p_key);
	
	/**
     * @brief Places a pixel with the Standard outline type.
     * @param p_atlasData The atlas data.
     * @param p_atlasSize Size of the atlas.
     * @param p_pixelOrigin The origin of the pixel.
     * @param p_x X-coordinate.
     * @param p_y Y-coordinate.
     * @param p_key Font key information.
     */
	void _placePixelStandard(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_pixelOrigin, const int &p_x, const int &p_y, const spk::Font::Key &p_key);
	
	/**
     * @brief Places a pixel with the Manhattan outline type.
     * @param p_atlasData The atlas data.
     * @param p_atlasSize Size of the atlas.
     * @param p_pixelOrigin The origin of the pixel.
     * @param p_x X-coordinate.
     * @param p_y Y-coordinate.
     * @param p_key Font key information.
     */
	void _placePixelManhattan(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Vector2Int& p_pixelOrigin, const int &p_x, const int &p_y, const spk::Font::Key &p_key);

	//! Type alias for the pixel placement function.
	using PlacePixelFunction = std::function<void(std::vector<uint8_t> &, const spk::Vector2Int &, const spk::Vector2Int&, const spk::Font::Key &)>;

	/**
     * @brief Requests the appropriate pixel placement function based on outline type.
     * @param p_outlineType The type of outline.
     * @return The appropriate pixel placement function.
     */
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

	/**
     * @brief Checks if a pixel is on the border.
     * @param p_atlasData The atlas data.
     * @param p_atlasSize Size of the atlas.
     * @param p_index Index of the pixel.
     * @return True if the pixel is on the border, false otherwise.
     */
	bool _isPixelOnBorder(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const size_t& p_index)
	{
		return (p_atlasData[p_index] == Font::CHAR_PIXEL &&
			(p_atlasData[p_index + 1] != Font::CHAR_PIXEL ||
			 p_atlasData[p_index - 1] != Font::CHAR_PIXEL ||
			 p_atlasData[p_index + p_atlasSize.y] != Font::CHAR_PIXEL ||
			 p_atlasData[p_index - p_atlasSize.y] != Font::CHAR_PIXEL)
		);
	}

	/**
     * @brief Applies the appropriate outline to the atlas data.
     * @param p_atlasData The atlas data.
     * @param p_atlasSize Size of the atlas.
     * @param p_key Font key information.
     */
	void _applyOutline(std::vector<uint8_t> &p_atlasData, const spk::Vector2Int &p_atlasSize, const spk::Font::Key &p_key)
	{
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
	}
}