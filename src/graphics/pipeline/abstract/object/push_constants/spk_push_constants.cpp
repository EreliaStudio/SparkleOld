#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	AbstractPipeline::Object::PushConstants::PushConstants(const spk::ShaderLayout::PushConstantsLayout& p_pushConstantsLayout) :
		FieldMap(p_pushConstantsLayout.stride())
	{
		for (const auto& field : p_pushConstantsLayout.fields())
		{
			insertNewField(spk::to_wstring(field.name), field.offset, field.data.size * field.data.format);
		}
	}
}