#include "graphics/pipeline/opengl/spk_gpu_object.hpp"
#include "spk_basic_functions.hpp"

namespace spk::GPU
{
	Buffer::Buffer(const Buffer::Mode& p_mode) :
		_mode(_convertModeToGLenum(p_mode))
	{
		glGenBuffers(1, &_vbo);
	}

	GLenum Buffer::_convertModeToGLenum(const Buffer::Mode &p_input)
	{
		switch (p_input)
		{
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

	void Buffer::push(const void* data, const size_t dataSize)
	{
		activate();
		_size = dataSize;

		if (_pushedSize < dataSize)
		{
			glBufferData(_mode, dataSize, data, GL_DYNAMIC_DRAW);
			_pushedSize = dataSize;
		}
		else
		{
			glBufferSubData(_mode, 0, dataSize, data);
		}
	}

	void Buffer::activate()
	{
		glBindBuffer(_mode, _vbo);
	}

	void Buffer::deactivate()
	{
		glBindBuffer(_mode, 0);
	}
}