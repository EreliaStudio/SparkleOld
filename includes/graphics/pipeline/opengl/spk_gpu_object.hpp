#pragma once

#include "graphics/pipeline/spk_shader_layout.hpp"
#include "spk_system_define.hpp"

namespace spk::GPU
{
    /**
     * @brief Manages the activation and deactivation of the Aggregator for OpenGL.
     */
    class Aggregator
    {
    private:
        GLuint _VAO; /**< Vertex Array Object */

    public:
        /**
         * @brief Default constructor.
         */
        Aggregator();

        /**
         * @brief Activates the Aggregator.
         */
        void activate();

        /**
         * @brief Deactivates the Aggregator.
         */
        void deactivate();

        /**
         * @brief Gets the Vertex Array Object identifier
        */
        GLuint vao() const;
    };

    /**
     * @brief Class representing an OpenGL buffer.
     */
    class Buffer
    {
    public:
        /**
         * @brief Possible modes for the Buffer.
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
         * @brief Converts the Mode into GLenum for OpenGL.
         */
        GLenum _convertModeToGLenum(const Mode &p_input);

    public:
        /**
         * @brief Constructor.
         */
        Buffer(const Mode &p_mode);
        
        /**
         * @brief Gets the Buffer Object identifier.
         * @return The VBO of the buffer.
        */
        GLuint vbo() const;

        /**
         * @brief Check the buffer type, to certify its validity
         * @return True if the buffer mode is different from Error, false if it's equal.
        */
        bool isValid() const;

        /**
         * @brief Pushes data into the buffer.
         */
        void push(const void *p_data, const size_t p_dataSize);

        /**
         * @brief Activates the buffer.
         */
        virtual void activate();

        /**
         * @brief Deactivates the buffer.
         */
        virtual void deactivate();
    };

    /**
     * @brief Manages StorageBuffers for OpenGL.
     */
    class StorageBuffer
    {
    private:
        GPU::Buffer _verticesBuffer; /**< Vertex Buffer */
        GPU::Buffer _indexesBuffer; /**< Index Buffer */

    public:
        /**
         * @brief Default constructor.
         */
        StorageBuffer();

        /**
         * @brief Gets the vertices Buffer.
         */
        const GPU::Buffer& verticesBuffer() const;


        /**
         * @brief Gets the indexes Buffer.
         */
        const GPU::Buffer& indexesBuffer() const;

        /**
         * @brief Adds a storage attribute.
         */
        void addStorageAttribute(const GLint& p_location, const GLint& p_format, const GLenum& p_type, const size_t& p_offset, const size_t& p_stride);

        /**
         * @brief Pushes vertex data into the buffer.
         */
        void pushVerticesData(const uint8_t* p_data, size_t p_dataSize);

        /**
         * @brief Pushes index data into the buffer.
         */
        void pushIndexesData(const uint8_t* p_data, size_t p_dataSize);

        /**
         * @brief Activates the StorageBuffers.
         */
        void activate();

        /**
         * @brief Deactivates the StorageBuffers.
         */
        void deactivate();
    };

    /**
     * @brief Manages an UniformBlockBuffer for OpenGL.
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
         * @brief Constructor.
         */
        UniformBlockBuffer(const GLuint& p_program, const std::wstring& p_uniformType, const size_t& p_blockBinding);

        /**
         * @brief Gets the index of the uniform block.
        */
        const GLuint& blockIndex() const;

        /**
         * @brief Activates the UniformBlockBuffer.
         */
        void activate() override;

        /**
         * @brief Deactivates the UniformBlockBuffer.
         */
        void deactivate() override;
    };
}
