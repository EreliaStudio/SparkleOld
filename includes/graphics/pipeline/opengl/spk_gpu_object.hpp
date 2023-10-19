#pragma once

#include "graphics/pipeline/spk_shader_layout.hpp"
#include "spk_system_define.hpp"

namespace spk::GPU
{
	class Aggregator
	{
	private:
		GLuint _VAO;

	public:
		Aggregator();

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
			UniformBlock
		};

	protected:
		GLuint _vbo;
		GLenum _mode;
		size_t _size = 0;
		size_t _pushedSize = 0;

		GLenum _convertModeToGLenum(const Mode &p_input);

	public:
		Buffer(const Mode &p_mode);

		void push(const void *data, const size_t dataSize);

		virtual void activate();
		virtual void deactivate();
	};

	class StorageBuffer
	{
	private:
		GPU::Buffer _verticesBuffer;
		GPU::Buffer _indexesBuffer;

	public:
		StorageBuffer();

		void addStorageAttribute(const GLint& p_location, const GLint& p_format, const GLenum& p_type, const size_t& p_offset, const size_t& p_stride);

		void pushVerticesData(const uint8_t* p_data, size_t p_dataSize);
		void pushIndexesData(const uint8_t* p_data, size_t p_dataSize);

		void activate();
		void deactivate();
	};

	class UniformBlockBuffer : public GPU::Buffer
	{
	private:
		GLuint _blockIndex;
		size_t _blockBinding;

	public:
		UniformBlockBuffer(const GLuint& p_program, const std::wstring& p_uniformType, const size_t& p_blockBinding);
		void activate() override;
		void deactivate() override;
	};
}