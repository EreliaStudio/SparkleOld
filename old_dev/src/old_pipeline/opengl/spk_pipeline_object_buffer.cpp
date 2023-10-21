#include "graphics/pipeline/opengl/spk_pipeline.hpp"
#include "spk_basic_functions.hpp"

namespace spk
{
	Pipeline::Buffer::Buffer(const Pipeline::Buffer::Mode& p_mode) :
		_mode(_convertModeToGLenum(p_mode))
	{
		
	}

	GLenum Pipeline::Buffer::_convertModeToGLenum(const spk::Pipeline::Buffer::Mode &p_input)
	{
		switch (p_input)
		{
		case (Mode::Data):
			return (GL_ARRAY_BUFFER);
		case (Mode::Indexes):
			return (GL_ELEMENT_ARRAY_BUFFER);
		case (Mode::PushConstant):
			return (GL_UNIFORM_BUFFER);
		}
		spk::throwException(L"Unexpected buffer mode [" + std::to_wstring(static_cast<size_t>(p_input)) + L"]");
		return (GL_NONE);
	}

	GLenum Pipeline::Buffer::_convertAttributeTypeToGLenum(const spk::ShaderLayout::Data::Type &p_input)
	{
		switch (p_input)
		{
		case (spk::ShaderLayout::Data::Type::Int):
			return (GL_INT);
		case (spk::ShaderLayout::Data::Type::UInt):
			return (GL_UNSIGNED_BYTE);
		case (spk::ShaderLayout::Data::Type::Float):
			return (GL_FLOAT);
		case (spk::ShaderLayout::Data::Type::Complex):
			return (GL_UNSIGNED_BYTE);
		}
		spk::throwException(L"Unexpected data type [" + std::to_wstring(static_cast<size_t>(p_input)) + L"]");
		return (GL_NONE);
	}

	Pipeline::Buffer::Buffer(const Pipeline::Buffer::Mode& p_mode, const spk::ShaderLayout::StorageLayout& p_configuration)
	{
		_mode = _convertModeToGLenum(p_mode);

		glGenBuffers(1, &_vbo);
		glBindBuffer(_mode, _vbo);
		for (const auto& field : p_configuration.fields)
		{
			glEnableVertexAttribArray(field.location);

			glVertexAttribPointer(
				field.location,
				field.attribute.format,
				_convertAttributeTypeToGLenum(field.attribute.type),
				GL_FALSE,
				p_configuration.stride,
				(void*)(field.offset));
		}
	}

	void Pipeline::Buffer::push(const void* data, const size_t dataSize)
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

	void Pipeline::Buffer::activate()
	{
		glBindBuffer(_mode, _vbo);
	}

	void Pipeline::Buffer::deactivate()
	{
		glBindBuffer(_mode, 0);
	}
}