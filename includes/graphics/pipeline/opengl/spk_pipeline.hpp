#pragma once

#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	class Pipeline : public spk::AbstractPipeline
	{
	public:
		class OpenGLObject : public Object
		{
		private:
			void _pushVerticesData(const uint8_t* p_data, size_t p_dataSize);
			void _pushIndexesData(const uint8_t* p_data, size_t p_dataSize);
			void _pushPushConstantsData(const uint8_t* p_data, size_t p_dataSize);
			void _onRender();
			
		public:
			OpenGLObject(AbstractPipeline* p_owner, const spk::ShaderLayout::StorageBufferLayout& p_storageBufferLayout, const spk::ShaderLayout::PushConstantsLayout& p_pushConstantsLayout);
		};

	private:
		GLuint _program;
		
		void _loadProgram(const spk::ShaderLayout& p_shaderLayout);

		std::shared_ptr<Object> _loadObject(const ShaderLayout::StorageBufferLayout& p_storageLayout, const ShaderLayout::PushConstantsLayout& p_pushConstantsLayout);

	public:
		Pipeline(const spk::ShaderModule &p_vertexInput, const spk::ShaderModule &p_fragmentInput);

		void launch(const size_t& p_nbVertex);

		void activate();
		void deactivate();
	};
}