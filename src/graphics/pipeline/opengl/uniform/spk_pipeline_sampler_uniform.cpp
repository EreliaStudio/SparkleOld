#include "graphics/pipeline/spk_pipeline.hpp"

namespace spk
{
	void Pipeline::OpenGLSamplerUniform::_pushData()
	{

	}

	Pipeline::OpenGLSamplerUniform::OpenGLSamplerUniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) :
		SamplerUniform(p_uniformBlockLayout)
	{

	}

	std::shared_ptr<Pipeline::SamplerUniform> Pipeline::_loadSamplerUniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout)
	{
		return (std::make_shared<OpenGLSamplerUniform>(p_uniformBlockLayout));
	}
}