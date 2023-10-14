#pragma once

#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	class Pipeline : public spk::AbstractPipeline
	{
	public:
		class OpenGLObject : public spk::AbstractPipeline::Object
		{
		private:
			void _pushStorageDatas(const void *p_data, size_t p_size);
			void _pushConstantsDatas(const void *p_data, size_t p_size);
			void _pushIndexesDatas(const void* p_data, size_t p_size);
			void _onRender();
			
		public:
			OpenGLObject(Pipeline* p_owner, const spk::ShaderLayout::StorageBufferLayout& p_storageLayout, const spk::ShaderLayout::PushConstantLayout& p_pushConstantsLayout);
		};

		class OpenGLUniformBlock : public spk::AbstractPipeline::UniformBlock
		{
		private:
			void _updateData(const void* p_data, size_t p_dataSize);

		public:
			OpenGLUniformBlock(const spk::ShaderLayout::UniformBlockLayout& p_uniformBlockLayout);

			void activate();
			void deactivate();
		};

		class OpenGLSamplerUniform : public spk::AbstractPipeline::SamplerUniform
		{
		private:
			void _updateSamplerID(int p_samplerID);

		public:
			OpenGLSamplerUniform(const spk::ShaderLayout::UniformBlockLayout& p_uniformBlockLayout);

			void activate();
			void deactivate();
		};

	private:
		GLuint _program;
		
		std::shared_ptr<UniformBlock> _loadUniformBlock(const spk::ShaderLayout::UniformBlockLayout& p_uniformBlockLayout);
		std::shared_ptr<SamplerUniform> _loadSamplerUniform(const spk::ShaderLayout::UniformBlockLayout& p_uniformBlockLayout);
		std::shared_ptr<Object> _loadObject(const spk::ShaderLayout::StorageBufferLayout& p_storageLayout, const spk::ShaderLayout::PushConstantLayout& p_pushConstantsLayout);

		void _loadProgram(const spk::ShaderLayout& p_shaderLayout);

	public:
		Pipeline(const spk::ShaderModule &p_vertexInput, const spk::ShaderModule &p_fragmentInput);

		void launch(const size_t& p_nbVertex);

		void activate();
		void deactivate();
	};
}