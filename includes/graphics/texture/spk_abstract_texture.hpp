#pragma once

#include <iostream>

namespace spk
{
	/**
	 * @brief Represents an abstract texture in the Sparkle framework.
	 * This class serves as the base class for all textures and defines the basic properties and behaviors of a texture.
	 */
	class AbstractTexture
	{
	public:
		/**
		 * @brief Texture formats.
		 * Defines the different color channels a texture can have.
		 */
		enum class Format
		{
			RGB,         ///< RGB format.
			RGBA,        ///< RGBA format with an additional alpha channel.
			BGR,         ///< BGR format.
			BGRA,        ///< BGRA format with an additional alpha channel.
			R,           ///< Single red channel format.
			GreyLevel    ///< Single channel format representing grayscale.
		};

		/**
		 * @brief Texture filtering options.
		 * Defines the various filtering methods that can be applied when sampling the texture.
		 */
		enum class Filtering
		{
			Nearest,              ///< Nearest neighbor filtering.
			Linear,               ///< Linear interpolation filtering.
			NearestMipmapNearest, ///< Nearest neighbor filtering with nearest mipmap.
			LinearMipmapLinear    ///< Linear interpolation with linear mipmap.
		};

		/**
		 * @brief Texture wrapping modes.
		 * Defines how the texture behaves when sampled outside the [0,1] texture coordinate range.
		 */
		enum class Wrap
		{
			Repeat,       ///< Repeat the texture in the respective axis.
			ClampToEdge,  ///< Clamps the texture coordinates between 0 and 1.
			ClampToBorder ///< Uses the border color for coordinates outside the texture.
		};

		/**
		 * @brief Mipmap settings for textures.
		 * Defines whether mipmaps should be used for the texture.
		 */
		enum class Mipmap
		{
			Disable, ///< Mipmapping is disabled.
			Enable   ///< Mipmapping is enabled.
		};

		/**
		 * @brief Default constructor.
		 */
		AbstractTexture() = default;

		/**
		 * @brief Copy constructor. Copying is not allowed for this class.
		 * @param p_other The other AbstractTexture object.
		 */
		AbstractTexture(const AbstractTexture& p_other) = delete;

		/**
		 * @brief Copy assignment operator. Copy assignment is not allowed for this class.
		 * @param p_other The other AbstractTexture object.
		 * @return Reference to the current object.
		 */
		AbstractTexture& operator = (const AbstractTexture& p_other) = delete;

		/**
		 * @brief Bind the texture to a specific texture index.
		 * @param p_textureIndex The index to which the texture should be bound.
		 */
		virtual void bind(int p_textureIndex) const = 0;

		/**
		 * @brief Unbind the texture.
		 */
		virtual void unbind() const = 0;

		// Friend functions to allow printing of Format enum to a wide output stream.
		friend std::wostream& operator << (std::wostream& p_os, const Format& p_format);
		// Friend functions to allow printing of Filtering enum to a wide output stream.
		friend std::wostream& operator << (std::wostream& p_os, const Filtering& p_format);
		// Friend functions to allow printing of Wrap enum to a wide output stream.
		friend std::wostream& operator << (std::wostream& p_os, const Wrap& p_format);
		// Friend functions to allow printing of Mipmap enum to a wide output stream.
		friend std::wostream& operator << (std::wostream& p_os, const Mipmap& p_format);
	};
}
