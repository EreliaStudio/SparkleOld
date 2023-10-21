#pragma once

#include "graphics/pipeline/spk_abstract_pipeline.hpp"
#include "graphics/pipeline/opengl/spk_gpu_object.hpp"

namespace spk
{
	class Pipeline : public spk::AbstractPipeline
	{
	public:
		class OpenGLObject : public Object
		{
		private:
			GPU::Aggregator _aggregator;
			GPU::StorageBuffer _storageBuffer;
			GPU::UniformBlockBuffer _pushConstantBuffer;

			void _pushVerticesData(const uint8_t* p_data, size_t p_dataSize);
			void _pushIndexesData(const uint8_t* p_data, size_t p_dataSize);
			void _pushPushConstantsData(const uint8_t* p_data, size_t p_dataSize);
			void _onRender();

			void _configureStorageBuffer(const ShaderLayout::StorageBufferLayout& p_storageBufferLayout);
			
		public:
			OpenGLObject(AbstractPipeline* p_owner, const ShaderLayout::StorageBufferLayout& p_storageBufferLayout, const ShaderLayout::PushConstantsLayout& p_pushConstantsLayout);
		};

		class OpenGLUniformBlock : public UniformBlock
		{
		private:
			GPU::UniformBlockBuffer _buffer;

			void _pushData();

		public:
			OpenGLUniformBlock(Pipeline* p_pipeline, const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout);
		};

		class OpenGLSamplerUniform : public SamplerUniform
		{
		private:
			void _pushData();

			GLint _location;		

		public:
			OpenGLSamplerUniform(Pipeline* p_owner, const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout);
		};

	private:
		GLuint _program;
		
		void _loadProgram(const ShaderLayout& p_shaderLayout);

		std::shared_ptr<UniformBlock> _loadUniformBlock(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout);
		std::shared_ptr<SamplerUniform> _loadSamplerUniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout);
		std::shared_ptr<Object> _loadObject(const ShaderLayout::StorageBufferLayout& p_storageLayout, const ShaderLayout::PushConstantsLayout& p_pushConstantsLayout);

	public:
		Pipeline(const spk::ShaderModule &p_vertexInput, const spk::ShaderModule &p_fragmentInput);

		void launch(const size_t& p_nbIndexes);

		void activate();
		void deactivate();
	};
}