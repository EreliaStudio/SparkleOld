#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	AbstractPipeline::UniformBlock::UniformBlock(const spk::ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) :
		AbstractPipeline::FieldMap(L"UniformBlock", p_uniformBlockLayout.stride())
	{
		for (const auto& field : p_uniformBlockLayout.fields())
		{
			_addField(spk::to_wstring(field.name), field.offset, field.data.size * field.data.format);
		}
	}

	void AbstractPipeline::UniformBlock::update()
	{

	}
}