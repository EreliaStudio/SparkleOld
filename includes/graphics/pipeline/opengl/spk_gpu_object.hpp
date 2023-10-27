#pragma once

#include "graphics/pipeline/spk_shader_layout.hpp"
#include "spk_system_define.hpp"

namespace spk::GPU
{
    /**
     * Manages the activation and deactivation of the Aggregator for OpenGL.
     */
    class Aggregator
    {
    private:
        GLuint _VAO; /**< Vertex Array Object */

    public:
        /**
         * Default constructor.
         */
        Aggregator();

        /**
         * Activates the Aggregator.
         */
        void activate();

        /**
         * Deactivates the Aggregator.
         */
        void deactivate();

        GLuint vao() const;
    };

    /**
     * Class representing an OpenGL buffer.
     */
    class Buffer
    {
    public:
        /**
         * Possible modes for the Buffer.
         */
        enum class Mode
        {
            Error, /**< Default state, related to an uninitialized buffer*/
            Vertices, /**< Vertex Buffer */
            Indexes, /**< Index Buffer */
            UniformBlock /**< Uniform Block Buffer */
        };

    protected:
        GLuint _vbo; /**< Buffer Object */
        GLenum _mode; /**< Buffer mode (GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, etc.) */
        size_t _size = 0; /**< Size of the buffer */
        size_t _pushedSize = 0; /**< Size of the data pushed into the buffer */

        /**
         * Converts the Mode into GLenum for OpenGL.
         */
        GLenum _convertModeToGLenum(const Mode &p_input);

    public:
        /**
         * Constructor.
         */
        Buffer(const Mode &p_mode);

        GLuint vbo() const;

        /**
         * @brief Check the buffer type, to certify its validity
         * @return True if the buffer mode is different from Error, false if it's equal.
        */
        bool isValid() const;

        /**
         * Pushes data into the buffer.
         */
        void push(const void *data, const size_t dataSize);

        /**
         * Activates the buffer.
         */
        virtual void activate();

        /**
         * Deactivates the buffer.
         */
        virtual void deactivate();
    };

    /**
     * Manages StorageBuffers for OpenGL.
     */
    class StorageBuffer
    {
    private:
        GPU::Buffer _verticesBuffer; /**< Vertex Buffer */
        GPU::Buffer _indexesBuffer; /**< Index Buffer */

    public:
        /**
         * Default constructor.
         */
        StorageBuffer();

        const GPU::Buffer& verticesBuffer() const;
        const GPU::Buffer& indexesBuffer() const;

        /**
         * Adds a storage attribute.
         */
        void addStorageAttribute(const GLint& p_location, const GLint& p_format, const GLenum& p_type, const size_t& p_offset, const size_t& p_stride);

        /**
         * Pushes vertex data into the buffer.
         */
        void pushVerticesData(const uint8_t* p_data, size_t p_dataSize);

        /**
         * Pushes index data into the buffer.
         */
        void pushIndexesData(const uint8_t* p_data, size_t p_dataSize);

        /**
         * Activates the StorageBuffers.
         */
        void activate();

        /**
         * Deactivates the StorageBuffers.
         */
        void deactivate();
    };

    /**
     * Manages an UniformBlockBuffer for OpenGL.
     */
    class UniformBlockBuffer : public GPU::Buffer
    {
    private:
        GLuint _blockIndex; /**< Uniform block index */
        size_t _blockBinding; /**< Uniform block binding */

    public:
        /**
         * @brief Default constructor
        */
        UniformBlockBuffer();

        /**
         * Constructor.
         */
        UniformBlockBuffer(const GLuint& p_program, const std::wstring& p_uniformType, const size_t& p_blockBinding);

        /**
         * Activates the UniformBlockBuffer.
         */
        void activate() override;

        /**
         * Deactivates the UniformBlockBuffer.
         */
        void deactivate() override;
    };
}
