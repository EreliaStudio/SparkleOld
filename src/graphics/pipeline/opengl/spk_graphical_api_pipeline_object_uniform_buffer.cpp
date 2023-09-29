#include "graphics/pipeline/opengl/spk_graphical_api_pipeline.hpp"
#include "spk_basic_functions.hpp"

namespace spk::GraphicalAPI
{
	Pipeline::OpenGLObject::UniformBuffer::UniformBuffer(const GLuint& p_program, const std::wstring& p_uniformType, const size_t& p_blockBinding) : 
		Pipeline::OpenGLObject::Buffer(Pipeline::OpenGLObject::Buffer::Mode::PushConstant),
		_blockBinding(p_blockBinding)
	{
		if (p_uniformType == L"")
			return ;


		_blockIndex = glGetUniformBlockIndex(p_program, spk::wstringToString(p_uniformType).c_str());
		if (_blockIndex == GL_INVALID_INDEX)
		{
			spk::throwException(L"Uniform block [" + p_uniformType + L"] isn't linked correctly : wrong index found");
		}

		glUniformBlockBinding(p_program, _blockIndex, _blockBinding);

		glGenBuffers(1, &_vbo);
		glBindBuffer(_mode, _vbo);
		glBindBufferBase(GL_UNIFORM_BUFFER, _blockBinding, _blockIndex);
	}
	
	void Pipeline::OpenGLObject::UniformBuffer::push(const void* data, const size_t dataSize)
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

	void Pipeline::OpenGLObject::UniformBuffer::activate()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, _vbo);
		glBindBufferBase(GL_UNIFORM_BUFFER, _blockBinding, _vbo);
	}
	
	void Pipeline::OpenGLObject::UniformBuffer::deactivate()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferBase(GL_UNIFORM_BUFFER, _blockBinding, 0);
	}
}