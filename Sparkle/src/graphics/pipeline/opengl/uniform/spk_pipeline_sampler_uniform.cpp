#include "graphics/pipeline/spk_pipeline.hpp"

namespace spk
{
	void Pipeline::OpenGLSamplerUniform::_pushData()
	{
		int value = *(reinterpret_cast<int*>(_data.data()));

		glUniform1i(_location, value);
	}

	Pipeline::OpenGLSamplerUniform::OpenGLSamplerUniform(Pipeline* p_owner, const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) :
		SamplerUniform(p_uniformBlockLayout)
	{
		glUseProgram(p_owner->_program);
		_location = glGetUniformLocation(p_owner->_program, spk::wstringToString(p_uniformBlockLayout.name()).c_str());

		if (_location == GL_INVALID_INDEX)
		{
			spk::throwException(L"Sampler uniform [" + p_uniformBlockLayout.name() + L"] isn't linked correctly : wrong location found");
		}
	}

	std::shared_ptr<Pipeline::SamplerUniform> Pipeline::_loadSamplerUniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout)
	{
		return (std::make_shared<OpenGLSamplerUniform>(this, p_uniformBlockLayout));
	}
}