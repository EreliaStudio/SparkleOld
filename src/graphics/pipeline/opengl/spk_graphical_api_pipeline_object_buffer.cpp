#include "graphics/pipeline/opengl/spk_graphical_api_pipeline.hpp"
#include "spk_basic_functions.hpp"

namespace spk::GraphicalAPI
{
	Pipeline::OpenGLObject::Buffer::Buffer(const spk::GraphicalAPI::AbstractPipeline::Object::Storage::Configuration& p_configuration)
	{
		spk::cout << L"Creating buffer with configuration:" << p_configuration << std::endl;
		_mode = spk::GraphicalAPI::convertModeToGLenum(p_configuration.mode);

		glGenBuffers(1, &_vbo);
		glBindBuffer(_mode, _vbo);
		for (const auto& attribute : p_configuration.attributes)
		{
			glEnableVertexAttribArray(attribute.second.location);

			glVertexAttribPointer(
				attribute.second.location,
				attribute.second.format,
				spk::GraphicalAPI::convertTypeToGLenum(attribute.second.type),
				GL_FALSE,
				p_configuration.stride,
				(void*)(attribute.second.offset));
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