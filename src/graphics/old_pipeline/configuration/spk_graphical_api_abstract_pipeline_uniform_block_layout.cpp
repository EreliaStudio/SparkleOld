#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{
	AbstractPipeline::Configuration::UniformBlockLayout::Field::Field() :
		offset(0),
		attribute()
	{

	}
	
	AbstractPipeline::Configuration::UniformBlockLayout::Field::Field(const AbstractPipeline::Configuration::Data& p_dataType, const size_t& p_offset) :
		offset(p_offset),
		attribute(p_dataType)
	{ 
	}

	AbstractPipeline::Configuration::UniformBlockLayout::UniformBlockLayout()
	{
		stride = 0;
	}

	AbstractPipeline::Configuration::UniformBlockLayout::UniformBlockLayout(const std::vector<Field> &p_fields) :
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