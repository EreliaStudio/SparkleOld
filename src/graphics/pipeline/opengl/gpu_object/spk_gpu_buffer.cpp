#include "graphics/pipeline/opengl/spk_gpu_object.hpp"
#include "spk_basic_functions.hpp"
#include "graphics/pipeline/spk_pipeline_basic_functions.hpp"

namespace spk::GPU
{
	Buffer::Buffer(const Buffer::Mode& p_mode) :
		_mode(_convertModeToGLenum(p_mode))
	{
		if (isValid() == true)
		{
			glGenBuffers(1, &_vbo);
		}
	}

	GLuint Buffer::vbo() const
	{
		return (_vbo);
	}
	
	bool Buffer::isValid() const
	{
		return (_mode != GL_NONE);
	}

	GLenum Buffer::_convertModeToGLenum(const Buffer::Mode &p_input)
	{
		switch (p_input)
		{
		case (Mode::Error):
			return (GL_NONE);
		case (Mode::Vertices):
			return (GL_ARRAY_BUFFER);
		case (Mode::Indexes):
			return (GL_ELEMENT_ARRAY_BUFFER);
		case (Mode::UniformBlock):
			return (GL_UNIFORM_BUFFER);
		}
		spk::throwException(L"Unexpected buffer mode [" + std::to_wstring(static_cast<size_t>(p_input)) + L"]");
		return (GL_NONE);
	}

	void Buffer::push(const void* p_data, const size_t p_dataSize)
	{
		if (isValid() == false)
			spk::throwException(L"Can't push an uninitialized buffer");

		activate();

		_size = p_dataSize;

		if (_pushedSize < p_dataSize)
		{
			glBufferData(_mode, p_dataSize, p_data, GL_DYNAMIC_DRAW);
			_pushedSize = p_dataSize;
		}
		else
		{
			glBufferSubData(_mode, 0, p_dataSize, p_data);
		}
	}

	void Buffer::activate()
	{
		if (isValid() == false)
			spk::throwException(L"Can't activate an uninitialized buffer");

		glBindBuffer(_mode, _vbo);
	}

	void Buffer::deactivate()
	{
		if (isValid() == false)
			spk::throwException(L"Can't deactivate an uninitialized buffer");

		glBindBuffer(_mode, 0);
	}
}