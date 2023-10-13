#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	AbstractPipeline::SamplerUniform::SamplerUniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) :
		_samplerID(-1)
	{

	}

	void AbstractPipeline::SamplerUniform::update()
	{
		_updateSamplerID(_samplerID);
	}

	AbstractPipeline::SamplerUniform& AbstractPipeline::SamplerUniform::operator=(int p_samplerID)
	{
		_samplerID = p_samplerID;
        return *this;
	}
	
	void AbstractPipeline::SamplerUniform::set(int p_samplerID)
	{
		_samplerID = p_samplerID;
	}
}