#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{

	ShaderLayout::StorageLayout::Field::Field() :
		location(0),
		offset(0),
		attribute()
	{

	}
	
	ShaderLayout::StorageLayout::Field::Field(const ShaderLayout::Data& p_dataType, const size_t& p_location, const size_t& p_offset) :
		location(p_location),
		offset(p_offset),
		attribute(p_dataType)
	{ 
	}

	ShaderLayout::StorageLayout::StorageLayout()
	{
		stride = 0;
	}

	ShaderLayout::StorageLayout::StorageLayout(const std::vector<Field> &p_fields) :
		StorageLayout()
	{
		for(auto field : p_fields)
		{
			field.offset = stride;
			fields.push_back(field);
		
			stride += field.attribute.size;
		}
	}
}