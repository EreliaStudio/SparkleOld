#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	AbstractPipeline::Uniform::Uniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) :
		_data(p_uniformBlockLayout.stride()),
		_name(p_uniformBlockLayout.name())
	{

	}

	const uint8_t *AbstractPipeline::Uniform::data() const
	{
		return (_data.data());
	}

	size_t AbstractPipeline::Uniform::size() const
	{
		return (_data.size());
	}
	
	void AbstractPipeline::Uniform::update()
	{
		_pushData();
	}
	
	void AbstractPipeline::Uniform::bind()
	{
		_bindUniform();
	}
}