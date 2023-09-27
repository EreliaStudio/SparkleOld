#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{
	AbstractPipeline::Configuration::StorageLayout::StorageLayout()
	{
		stride = 0;
	}

	AbstractPipeline::Configuration::StorageLayout::StorageLayout(const std::vector<Field> &p_fields) :
		StorageLayout()
	{
		for(auto field : p_fields)
		{
			field.attribute.offset = stride;
			fields.push_back(field);
		
			stride += field.attribute.format * field.attribute.unitSize;
		}
	}
}