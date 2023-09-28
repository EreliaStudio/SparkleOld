#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{
	AbstractPipeline::Configuration::PushConstantLayout::Field::Field() :
		offset(0),
		attribute()
	{

	}
	
	AbstractPipeline::Configuration::PushConstantLayout::Field::Field(const AbstractPipeline::Configuration::Data& p_dataType, const size_t& p_offset) :
		offset(p_offset),
		attribute(p_dataType)
	{ 
	}

	AbstractPipeline::Configuration::PushConstantLayout::PushConstantLayout()
	{
		stride = 0;
	}

	AbstractPipeline::Configuration::PushConstantLayout::PushConstantLayout(const std::vector<Field> &p_fields) :
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