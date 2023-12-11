#pragma once

#include "graphics/spk_texture.hpp"

namespace spk
{
    /**
     * @class Image
     * @brief Represents an image which is a specialized type of texture.
     * 
     * This class loads an image from a given file path and allows access to 
     * the raw texture data, size, and number of channels.
     */
	class Image : public Texture
	{
	private:
		uint8_t *_textureData; ///< Raw texture data.
		spk::Vector2Int _size; ///< Dimensions (width and height) of the image.
		int _channels; ///< Number of color channels in the image.

	public:
		/**
		 * @brief Constructs the Image object and loads the image from the specified path.
		 * 
		 * @param p_path Path to the image file.
		 */
		Image(const std::filesystem::path& p_path);
	};
}
