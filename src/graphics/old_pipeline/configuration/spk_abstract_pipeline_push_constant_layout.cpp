#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	ShaderLayout::PushConstantsLayout::Field::Field() :
		offset(0),
		name(L"Unnamed uniform"),
		attribute()
	{

	}
	
	ShaderLayout::PushConstantsLayout::Field::Field(const std::wstring& p_name, const ShaderLayout::Data& p_dataType, const size_t& p_offset) :
		offset(p_offset),
		name(p_name),
		attribute(p_dataType)
	{ 
	}

	ShaderLayout::PushConstantsLayout::PushConstantsLayout()
	{
		stride = 0;
	}

	ShaderLayout::PushConstantsLayout::PushConstantsLayout(const std::vector<Field> &p_fields) :
		PushConstantsLayout()
	{
		for(auto field : p_fields)
		{
			field.offset = stride;
			fields.push_back(field);
		
			stride += field.attribute.size;
		}
	}
}