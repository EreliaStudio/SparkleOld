#pragma once

#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"
#include "graphics/pipeline/spk_graphical_api_pipeline_basic_functions.hpp"

namespace spk::GraphicalAPI
{
	class Pipeline : public spk::GraphicalAPI::AbstractPipeline
	{
	private:
		class Aggregator
		{
		private:
			spk::GraphicalAPI::AbstractPipeline* _owner;
			GLuint _VAO;

		public:
			Aggregator(spk::GraphicalAPI::AbstractPipeline* p_owner);

			void activate();
			void deactivate();
		};

		class Buffer
		{
		public:
			enum class Mode
			{
				Vertices,
				Indexes,
				PushConstant
			};

		protected:
			GLuint _vbo;
			GLenum _mode;
			size_t _size = 0;
			size_t _pushedSize = 0;

			GLenum _convertModeToGLenum(const Mode &p_input);
			GLenum _convertAttributeTypeToGLenum(const spk::GraphicalAPI::ShaderLayout::Data::Type &p_input);
			
		public:
			Buffer(const Mode& p_mode);
			Buffer(const Pipeline::Buffer::Mode& p_mode, const ShaderLayout::StorageLayout& p_configuration);

			virtual void push(const void* data, const size_t dataSize);

			virtual void activate();
			virtual void deactivate();
		};

		class UniformBuffer : public Buffer
		{
		protected:
			GLuint _blockIndex;
			size_t _blockBinding;

		public:
			UniformBuffer(const GLuint& p_program, const std::wstring& p_uniformType, const size_t& p_blockBinding);

			void push(const void* data, const size_t dataSize);

			void activate();
			void deactivate();
		};

		class OpenGLObject : public spk::GraphicalAPI::AbstractPipeline::Object
		{
		private:
			Aggregator _aggregator;
			Buffer _modelBuffer;
			Buffer _indexesBuffer;
			UniformBuffer _pushConstantBuffer;

			size_t _findFirstBindingAvailible(const spk::GraphicalAPI::ShaderLayout& p_configuration);

		public:
			OpenGLObject(Pipeline* p_owner);

			void push();
			void updateConstants();

			void activate();
			void deactivate();
		};

		class OpenGLUniformBlock : public spk::GraphicalAPI::AbstractPipeline::UniformBlock
		{
		private:
			UniformBuffer _buffer;

		public:
			OpenGLUniformBlock(Pipeline* p_owner, const Configuration::UniformBlockLayout& p_layout) :
				spk::GraphicalAPI::AbstractPipeline::UniformBlock(p_layout),
				_buffer(p_owner->program(), p_layout.type, p_layout.binding)
			{	
				
			}

			void update()
			{
				_buffer.push(data(), size());
			}
		};

		GLuint _program;

		void _loadProgram(
			const std::string& p_vertexName, const std::string& p_vertexCode,
			const std::string& p_fragmentName, const std::string& p_fragmentCode);

		std::unique_ptr<UniformBlock> _createUniformBlock(const Configuration::UniformBlockLayout& p_layout);

	public:
		Pipeline(const std::filesystem::path& p_vertexShaderPath, const std::filesystem::path& p_fragmentShaderPath);

		void activate();
		void deactivate();

		void launch(const size_t& p_nbIndexes);

		const GLuint& program() const;

		std::unique_ptr<Object> createObject();
	};
}