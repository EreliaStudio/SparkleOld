#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{

	AbstractPipeline::Configuration::StorageLayout::Field::Field()
	{
		
	}
	
	AbstractPipeline::Configuration::StorageLayout::Field::Field(const AbstractPipeline::Configuration::Data& p_dataType, const size_t& p_location, const size_t& p_offset)
	{

	}

	AbstractPipeline::Configuration::StorageLayout::StorageLayout()
	{
		stride = 0;
	}

	AbstractPipeline::Configuration::StorageLayout::StorageLayout(const std::vector<Field> &p_fields) :
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