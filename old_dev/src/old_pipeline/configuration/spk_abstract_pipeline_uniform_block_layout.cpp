#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	ShaderLayout::UniformBlockLayout::Field::Field() :
		offset(0),
		attribute()
	{

	}
	
	ShaderLayout::UniformBlockLayout::Field::Field(const ShaderLayout::Data& p_dataType, const size_t& p_offset) :
		offset(p_offset),
		attribute(p_dataType)
	{ 
	}

	ShaderLayout::UniformBlockLayout::UniformBlockLayout()
	{
		stride = 0;
	}

	ShaderLayout::UniformBlockLayout::UniformBlockLayout(const std::vector<Field> &p_fields) :
		UniformBlockLayout()
	{
		for(auto field : p_fields)
		{
			field.offset = stride;
			fields.push_back(field);
		
			stride += field.attribute.size;
		}
	}
}