#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	void AbstractPipeline::SamplerUniform::_bindUniform()
	{
		_pushData();
	}
	
	AbstractPipeline::SamplerUniform::SamplerUniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) :
		AbstractPipeline::Uniform(p_uniformBlockLayout)
	{

	}
}