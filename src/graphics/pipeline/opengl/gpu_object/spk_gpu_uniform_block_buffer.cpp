#include "graphics/pipeline/opengl/spk_gpu_object.hpp"
#include "spk_basic_functions.hpp"

namespace spk::GPU
{ 
	UniformBlockBuffer::UniformBlockBuffer(const GLuint& p_program, const std::wstring& p_uniformType, const size_t& p_blockBinding) : 
		Buffer(Buffer::Mode::UniformBlock),
		_blockBinding(p_blockBinding)
	{
		if (p_uniformType == L"")
			spk::throwException(L"Uniform block can't have a blank name");

		_blockIndex = glGetUniformBlockIndex(p_program, spk::wstringToString(p_uniformType).c_str());
		if (_blockIndex == GL_INVALID_INDEX)
		{
			spk::throwException(L"Uniform block [" + p_uniformType + L"] isn't linked correctly : wrong index found");
		}

		glUniformBlockBinding(p_program, _blockIndex, _blockBinding);

		activate();
		glBindBufferBase(GL_UNIFORM_BUFFER, _blockBinding, _vbo);
	}

	void UniformBlockBuffer::activate()
	{
		glBindBuffer(_mode, _vbo);
	}
	
	void UniformBlockBuffer::deactivate()
	{
		glBindBuffer(_mode, 0);
	}
}