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

	AbstractPipeline::Uniform& AbstractPipeline::Uniform::operator<<(std::wostream& (*func)(std::wostream&)) {
		_pushData();
		return *this;
	}
}