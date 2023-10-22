#pragma once

#include <filesystem>
#include "math/spk_vector2.hpp"

namespace spk
{
    /**
     * @class AbstractImage
     * @brief Abstract class for managing images.
     * 
     * Provides a common interface for image manipulation, supporting various
     * texture filtering, wrapping, and mipmapping options.
     */
    class AbstractImage
    {
    public:
        /// Enum for different types of texture filtering.
        enum class Filtering
        {
            Nearest,
            Linear,
            NearestMipmapNearest,
            LinearMipmapLinear
        };

        /// Enum for texture wrapping modes.
        enum class Wrap
        {
            Repeat,
            ClampToEdge,
            ClampToBorder
        };

        /// Enum for mipmapping options.
        enum class Mipmap
        {
            Disable,
            Enable
        };

    protected:
        /// Pointer to image data.
        uint8_t* _imageData;

        /// Dimensions of the image.
        spk::Vector2Int _size;

        /// Number of color channels in the image.
        int _channels;

        /// Filtering option for the texture.
        Filtering _filtering;

        /// Wrapping option for the texture.
        Wrap _wrap;

        /// Mipmapping option for the texture.
        Mipmap _mipmap;

        /// Release the loaded image data.
        void _releaseImage();

    public:
        /**
         * @brief Constructs an AbstractImage object.
         * 
         * @param p_filePath File path of the image to load.
         * @param p_filtering Filtering mode for the image texture (default is Nearest).
         * @param p_wrap Wrapping mode for the image texture (default is Repeat).
         * @param p_mipmap Mipmapping option for the image texture (default is Disable).
         */
        AbstractImage(const std::filesystem::path& p_filePath, Filtering p_filtering = Filtering::Nearest, Wrap p_wrap = Wrap::Repeat, Mipmap p_mipmap = Mipmap::Disable);

        /// Alternative constructor with different parameter ordering.
        AbstractImage(const std::filesystem::path& p_filePath, Mipmap p_mipmap, Filtering p_filtering = Filtering::Nearest, Wrap p_wrap = Wrap::Repeat);
        
        /// Destructor to release image resources.
        ~AbstractImage();

        /**
         * @brief Loads an image from a file path.
         * 
         * @param p_filePath File path of the image to load.
         */
        void loadFromFile(const std::filesystem::path& p_filePath);

        /// Bind the image to a texture unit.
        virtual void bind(int p_textureIndex = 0) = 0;

        /// Unbind the image texture.
        virtual void unbind() = 0;

        /// Uploads image data to the GPU.
        virtual void uploadToGPU() = 0;

        /// Releases GPU resources related to the image.
        virtual void releaseGPUResources() = 0;

        /// Get the dimensions of the image.
        spk::Vector2Int size() const;

        /// Get the filtering mode for the texture.
        Filtering filtering() const;

        /// Get the wrapping mode for the texture.
        Wrap wrap() const;

        /// Get the mipmapping option for the texture.
        Mipmap mipmap() const;
    };
}
