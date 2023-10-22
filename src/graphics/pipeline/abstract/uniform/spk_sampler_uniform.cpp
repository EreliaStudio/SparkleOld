#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	AbstractPipeline::SamplerUniform::SamplerUniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) :
		AbstractPipeline::Uniform(p_uniformBlockLayout)
	{

	}
}