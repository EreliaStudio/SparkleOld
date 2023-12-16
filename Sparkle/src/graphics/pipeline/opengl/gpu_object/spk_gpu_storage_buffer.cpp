#include "graphics/pipeline/opengl/spk_gpu_object.hpp"

namespace spk::GPU
{
	StorageBuffer::StorageBuffer() :
		_verticesBuffer(Buffer::Mode::Vertices),
		_indexesBuffer(Buffer::Mode::Indexes)
	{

	}

	const GPU::Buffer& StorageBuffer::verticesBuffer() const
	{
		return (_verticesBuffer);
	}
	
	const GPU::Buffer& StorageBuffer::indexesBuffer() const
	{
		return (_indexesBuffer);
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
		if (_verticesBuffer.isValid() == true)
			_verticesBuffer.push(p_data, p_dataSize);
	}

	void StorageBuffer::pushIndexesData(const uint8_t* p_data, size_t p_dataSize)
	{
		if (_indexesBuffer.isValid() == true)
			_indexesBuffer.push(p_data, p_dataSize);
	}

	void StorageBuffer::activate()
	{
		if (_verticesBuffer.isValid() == true)
			_verticesBuffer.activate();
		if (_indexesBuffer.isValid() == true)
			_indexesBuffer.activate();
	}

	void StorageBuffer::deactivate()
	{
		if (_verticesBuffer.isValid() == true)
			_verticesBuffer.deactivate();
		if (_indexesBuffer.isValid() == true)
			_indexesBuffer.deactivate();
	}
}