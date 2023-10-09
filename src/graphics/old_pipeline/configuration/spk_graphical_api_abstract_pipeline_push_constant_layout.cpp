#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{
	ShaderLayout::PushConstantLayout::Field::Field() :
		offset(0),
		name(L"Unnamed uniform"),
		attribute()
	{

	}
	
	ShaderLayout::PushConstantLayout::Field::Field(const std::wstring& p_name, const ShaderLayout::Data& p_dataType, const size_t& p_offset) :
		offset(p_offset),
		name(p_name),
		attribute(p_dataType)
	{ 
	}

	ShaderLayout::PushConstantLayout::PushConstantLayout()
	{
		stride = 0;
	}

	ShaderLayout::PushConstantLayout::PushConstantLayout(const std::vector<Field> &p_fields) :
		PushConstantLayout()
	{
		for(auto field : p_fields)
		{
			field.offset = stride;
			fields.push_back(field);
		
			stride += field.attribute.size;
		}
	}
}