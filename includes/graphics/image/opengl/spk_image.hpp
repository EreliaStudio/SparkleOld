#pragma once

#include "graphics/image/spk_abstract_image.hpp"
#include "spk_system_define.hpp"

namespace spk
{
    /**
     * @class Image
     * @brief Concrete implementation of the AbstractImage class for OpenGL.
     * 
     * Manages the image as an OpenGL texture and provides methods for
     * binding, unbinding, and uploading to the GPU.
     */
    class Image : public AbstractImage
    {
    private:
        /// OpenGL texture ID.
        GLuint _textureId;

        /// OpenGL format for the texture.
        GLenum _format;

    public:
        /**
         * @brief Constructs an Image object.
         * 
         * @param p_filePath File path of the image to load.
         * @param p_filtering Filtering mode for the image texture (default is Nearest).
         * @param p_wrap Wrapping mode for the image texture (default is Repeat).
         * @param p_mipmap Mipmapping option for the image texture (default is Disable).
         */
        Image(const std::filesystem::path& p_filePath, Filtering p_filtering = Filtering::Nearest, Wrap p_wrap = Wrap::Repeat, Mipmap p_mipmap = Mipmap::Disable);

        /// Alternative constructor with different parameter ordering.
        Image(const std::filesystem::path& p_filePath, Mipmap p_mipmap, Filtering p_filtering = Filtering::Nearest, Wrap p_wrap = Wrap::Repeat);

        /// Destructor to release OpenGL texture resources.
        ~Image();

        /**
         * @brief Bind the image to a specific texture unit.
         * 
         * @param p_textureIndex Index of the texture unit to which the image will be bound (default is 0).
         */
        void bind(int p_textureIndex = 0) override;

        /// Unbind the image from the OpenGL texture unit.
        void unbind() override;

        /// Uploads image data to the GPU as an OpenGL texture.
        void uploadToGPU() override;

        /// Releases OpenGL texture resources.
        void releaseGPUResources() override;
    };
}
