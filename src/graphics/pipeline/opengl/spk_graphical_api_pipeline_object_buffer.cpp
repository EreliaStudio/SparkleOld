#include "graphics/pipeline/opengl/spk_graphical_api_pipeline.hpp"
#include "spk_basic_functions.hpp"

namespace spk::GraphicalAPI
{
	Pipeline::OpenGLObject::Buffer::Buffer(const Pipeline::OpenGLObject::Buffer::Mode& p_mode) :
		_mode(_convertModeToGLenum(p_mode))
	{
		
	}

	GLenum Pipeline::OpenGLObject::Buffer::_convertModeToGLenum(const spk::GraphicalAPI::Pipeline::OpenGLObject::Buffer::Mode &p_input)
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

	GLenum Pipeline::OpenGLObject::Buffer::_convertAttributeTypeToGLenum(const spk::GraphicalAPI::AbstractPipeline::Configuration::Data::Type &p_input)
	{
		switch (p_input)
		{
		case (spk::GraphicalAPI::AbstractPipeline::Configuration::Data::Type::Int):
			return (GL_INT);
		case (spk::GraphicalAPI::AbstractPipeline::Configuration::Data::Type::UInt):
			return (GL_UNSIGNED_BYTE);
		case (spk::GraphicalAPI::AbstractPipeline::Configuration::Data::Type::Float):
			return (GL_FLOAT);
		case (spk::GraphicalAPI::AbstractPipeline::Configuration::Data::Type::Complex):
			return (GL_UNSIGNED_BYTE);
		}
		spk::throwException(L"Unexpected data type [" + std::to_wstring(static_cast<size_t>(p_input)) + L"]");
		return (GL_NONE);
	}

	Pipeline::OpenGLObject::Buffer::Buffer(const Pipeline::OpenGLObject::Buffer::Mode& p_mode, const spk::GraphicalAPI::AbstractPipeline::Configuration::StorageLayout& p_configuration)
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

	void Pipeline::OpenGLObject::Buffer::push(const void* data, const size_t dataSize)
	{
		activate();
		_size = dataSize;
		glBindBuffer(_mode, _vbo);

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

	void Pipeline::OpenGLObject::Buffer::activate()
	{
		glBindBuffer(_mode, _vbo);
	}

	void Pipeline::OpenGLObject::Buffer::deactivate()
	{
		glBindBuffer(_mode, 0);
	}
}