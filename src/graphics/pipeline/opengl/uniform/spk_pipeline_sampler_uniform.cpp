#include "graphics/pipeline/spk_pipeline.hpp"

namespace spk
{
	std::shared_ptr<Pipeline::SamplerUniform> Pipeline::_loadSamplerUniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout)
	{
		return (std::make_shared<OpenGLSamplerUniform>(p_uniformBlockLayout));
	}
}