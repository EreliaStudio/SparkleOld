#pragma once

#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	class Pipeline : public spk::AbstractPipeline
	{
	public:
		class Aggregator
		{
		private:
			spk::AbstractPipeline* _owner;
			GLuint _VAO;

		public:
			Aggregator(spk::AbstractPipeline* p_owner);

			void activate();
			void deactivate();
		};

		class Buffer
		{
		public:
			enum class Mode
			{
				Data,
				Indexes,
				PushConstant
			};

		protected:
			GLuint _vbo;
			GLenum _mode;
			size_t _size = 0;
			size_t _pushedSize = 0;

			GLenum _convertModeToGLenum(const Mode &p_input);
			GLenum _convertAttributeTypeToGLenum(const spk::ShaderLayout::Data::Type &p_input);
			
		public:
			Buffer(const Mode& p_mode);
			Buffer(const Pipeline::Buffer::Mode& p_mode, const ShaderLayout::StorageBufferLayout& p_configuration);

			virtual void push(const void* data, const size_t dataSize);

			virtual void activate();
			virtual void deactivate();
		};

		class OpenGLObject : public Object
		{
		private:
			Aggregator _aggregator;
			Buffer _storageBuffer;
			Buffer _indexesBuffer;

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

		void launch(const size_t& p_nbIndexes);

		void activate();
		void deactivate();
	};
}