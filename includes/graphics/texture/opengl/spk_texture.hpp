#pragma once

#include "spk_system_define.hpp"
#include "math/spk_vector2.hpp"
#include "graphics/texture/spk_abstract_texture.hpp"

namespace spk
{
	/**
	 * @brief Represents a concrete implementation of a texture in OpenGL framework.
	 * 
	 * This class provides a concrete texture representation, derived from the AbstractTexture class. 
	 * It provides functionalities for uploading texture data to the GPU and managing GPU memory.
	 */
	class Texture : public AbstractTexture
	{
	private:
		bool _loaded;     ///< Flag indicating if the texture is loaded into GPU memory.
		GLuint _textureID; ///< The OpenGL ID for the texture.

	public:
		/**
		 * @brief Default constructor. Initializes an empty texture.
		 */
		Texture();

		/**
		 * @brief Constructor to initialize a texture with provided data and properties.
		 * 
		 * @param p_textureData Pointer to the texture data.
		 * @param p_textureSize Dimensions of the texture.
		 * @param p_format Color format of the texture.
		 * @param p_filtering Filtering method to be used for the texture.
		 * @param p_wrap Wrapping mode for the texture.
		 * @param p_mipmap Mipmap setting for the texture.
		 */
		Texture(const uint8_t* p_textureData, const spk::Vector2UInt& p_textureSize,
			const Format& p_format, const Filtering& p_filtering,
			const Wrap& p_wrap, const Mipmap& p_mipmap);

		/**
		 * @brief Destructor. Releases any GPU resources if occupied.
		 */
		~Texture();

		/**
		 * @brief Uploads the provided texture data to the GPU.
		 * 
		 * @param p_textureData Pointer to the texture data.
		 * @param p_textureSize Dimensions of the texture.
		 * @param p_format Color format of the texture.
		 * @param p_filtering Filtering method to be used for the texture.
		 * @param p_wrap Wrapping mode for the texture.
		 * @param p_mipmap Mipmap setting for the texture.
		 */
		void uploadToGPU(const uint8_t* p_textureData, const spk::Vector2UInt& p_textureSize,
			const Format& p_format, const Filtering& p_filtering,
			const Wrap& p_wrap, const Mipmap& p_mipmap);

		/**
		 * @brief Releases the GPU memory occupied by this texture.
		 */
		void releaseGPUMemory();

		/**
		 * @brief Bind the texture to a specific texture index.
		 * 
		 * @param p_textureIndex The index to which the texture should be bound. Default is 0.
		 */
		void bind(int p_textureIndex = 0) const;

		/**
		 * @brief Unbind the texture.
		 */
		void unbind() const;
	};
}
