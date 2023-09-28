#pragma once

#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"
#include "graphics/pipeline/spk_graphical_api_pipeline_basic_functions.hpp"

namespace spk::GraphicalAPI
{
	class Pipeline : public spk::GraphicalAPI::AbstractPipeline
	{
	private:
		class OpenGLObject : public spk::GraphicalAPI::AbstractPipeline::Object
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
					Data,
					Indexes
				};

			private:
				GLuint _vbo;
				GLenum _mode;
				size_t _size = 0;
				size_t _pushedSize = 0;

				GLenum _convertModeToGLenum(const Mode &p_input);
			public:
				Buffer(const Mode& p_mode);
				Buffer(const Pipeline::OpenGLObject::Buffer::Mode& p_mode, const AbstractPipeline::Configuration::StorageLayout& p_configuration);

				void push(const void* data, const size_t dataSize);

				void activate();
				void deactivate();
			};

			Aggregator _aggregator;
			Buffer _modelBuffer;
			Buffer _indexesBuffer;

		public:
			OpenGLObject(spk::GraphicalAPI::AbstractPipeline* p_owner);

			void push();

			void activate();
			void deactivate();
		};

		GLuint _program;

		void _loadProgram(
			const std::string& p_vertexName, const std::string& p_vertexCode,
			const std::string& p_fragmentName, const std::string& p_fragmentCode);

	public:
		Pipeline(const std::filesystem::path& p_vertexShaderPath, const std::filesystem::path& p_fragmentShaderPath);

		void activate();
		void deactivate();

		void launch(const size_t& p_nbIndexes);

		std::shared_ptr<Object> createObject();
	};
}