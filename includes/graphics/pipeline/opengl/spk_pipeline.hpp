#pragma once

#include "graphics/pipeline/spk_abstract_pipeline.hpp"
#include "graphics/pipeline/opengl/spk_gpu_object.hpp"

namespace spk
{
    /**
     * Derived class from AbstractPipeline specialized for OpenGL.
     */
    class Pipeline : public spk::AbstractPipeline
    {
    public:
        /**
         * Class representing an OpenGL Object.
         */
        class OpenGLObject : public Object
        {
        private:
            GPU::Aggregator _aggregator; /**< Aggregator for OpenGL */
            GPU::StorageBuffer _storageBuffer; /**< Storage buffer for vertex and index data */
            GPU::UniformBlockBuffer _pushConstantBuffer; /**< Buffer for push constants */

            /**
             * Internal function to push vertex data into the buffer.
             */
            void _pushVerticesData(const uint8_t* p_data, size_t p_dataSize);

            /**
             * Internal function to push index data into the buffer.
             */
            void _pushIndexesData(const uint8_t* p_data, size_t p_dataSize);

            /**
             * Internal function to push push constant data into the buffer.
             */
            void _pushPushConstantsData(const uint8_t* p_data, size_t p_dataSize);

            /**
             * Internal function to handle rendering.
             */
            void _onRender();

            /**
             * Configures the storage buffer.
             */
            void _configureStorageBuffer(const ShaderLayout::StorageBufferLayout& p_storageBufferLayout);
            
        public:
            /**
             * Constructor for OpenGLObject.
             */
            OpenGLObject(AbstractPipeline* p_owner, const ShaderLayout::StorageBufferLayout& p_storageBufferLayout, const ShaderLayout::PushConstantsLayout& p_pushConstantsLayout);
        };

        /**
         * Class representing an OpenGL Uniform Block.
         */
        class OpenGLUniformBlock : public UniformBlock
        {
        private:
            GPU::UniformBlockBuffer _buffer; /**< Buffer for uniform data */

            /**
             * Internal function to push uniform data into the buffer.
             */
            void _pushData();

        public:
            /**
             * Constructor for OpenGLUniformBlock.
             */
            OpenGLUniformBlock(Pipeline* p_pipeline, const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout);
        };

        /**
         * Class representing an OpenGL Sampler Uniform.
         */
        class OpenGLSamplerUniform : public SamplerUniform
        {
        private:
            /**
             * Internal function to push sampler data.
             */
            void _pushData();

            GLint _location; /**< Location of the sampler in the shader program */

        public:
            /**
             * Constructor for OpenGLSamplerUniform.
             */
            OpenGLSamplerUniform(Pipeline* p_owner, const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout);
        };

    private:
        GLuint _program; /**< Shader program handle */

        /**
         * Load a shader program.
         */
        void _loadProgram(const ShaderLayout& p_shaderLayout);

        /**
         * Load a uniform block.
         */
        std::shared_ptr<UniformBlock> _loadUniformBlock(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout);

        /**
         * Load a sampler uniform.
         */
        std::shared_ptr<SamplerUniform> _loadSamplerUniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout);

        /**
         * Load an object for rendering.
         */
        std::shared_ptr<Object> _loadObject(const ShaderLayout::StorageBufferLayout& p_storageLayout, const ShaderLayout::PushConstantsLayout& p_pushConstantsLayout);

    public:
        /**
         * Constructor for Pipeline.
         */
        Pipeline(const spk::ShaderModule &p_vertexInput, const spk::ShaderModule &p_fragmentInput);

        /**
         * Launch the rendering operation.
         */
        void launch(const size_t& p_nbIndexes);

        /**
         * Activate the pipeline.
         */
        void activate();

        /**
         * Deactivate the pipeline.
         */
        void deactivate();
    };
}
