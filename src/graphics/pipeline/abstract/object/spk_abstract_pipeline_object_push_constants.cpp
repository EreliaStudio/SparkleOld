#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	AbstractPipeline::Object::PushConstants::PushConstants(const ShaderLayout::PushConstantLayout& p_pushConstantsLayout) :
		FieldMap(L"PushConstants", p_pushConstantsLayout.stride())
	{
		for (const auto& field : p_pushConstantsLayout.fields())
		{
			_addField(spk::to_wstring(field.name), field.offset, field.data.size * field.data.format);
		}
	}
}
