#include "graphics/pipeline/opengl/spk_gpu_object.hpp"

namespace spk::GPU
{
	StorageBuffer::StorageBuffer() :
		_verticesBuffer(Buffer::Mode::Vertices),
		_indexesBuffer(Buffer::Mode::Indexes)
	{

	}

	void StorageBuffer::addStorageAttribute(const GLint &p_location, const GLint &p_format, const GLenum &p_type, const size_t &p_offset, const size_t &p_stride)
	{
		_verticesBuffer.activate();
		glEnableVertexAttribArray(p_location);

		glVertexAttribPointer(
			p_location,
			p_format,
			p_type,
			GL_FALSE,
			p_stride,
			(void *)(p_offset));
	}

	void StorageBuffer::pushVerticesData(const uint8_t* p_data, size_t p_dataSize)
	{
		_verticesBuffer.push(p_data, p_dataSize);
	}

	void StorageBuffer::pushIndexesData(const uint8_t* p_data, size_t p_dataSize)
	{
		_indexesBuffer.push(p_data, p_dataSize);
	}

	void StorageBuffer::activate()
	{
		_verticesBuffer.activate();
		_indexesBuffer.activate();
	}

	void StorageBuffer::deactivate()
	{
		_verticesBuffer.deactivate();
		_indexesBuffer.deactivate();
	}
}