#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{
	bool AbstractPipeline::Configuration::UniformBlockLayout::Block::Key::operator<(const Key& other) const
	{
		if (set < other.set)
		{
			return true;
		}
		else if (set == other.set)
		{
			if (binding < other.binding)
			{
				return true;
			}
		}
		return false;
	}

	AbstractPipeline::Configuration::UniformBlockLayout::Block::Field::Field() :
		offset(0),
		attribute()
	{

	}
	
	AbstractPipeline::Configuration::UniformBlockLayout::Block::Field::Field(const AbstractPipeline::Configuration::Data& p_dataType, const size_t& p_offset) :
		offset(p_offset),
		attribute(p_dataType)
	{ 
	}

	AbstractPipeline::Configuration::UniformBlockLayout::Block::Block()
	{
		stride = 0;
	}

	AbstractPipeline::Configuration::UniformBlockLayout::Block::Block(const std::vector<Field> &p_fields) :
		Block()
	{
		for(auto field : p_fields)
		{
			field.offset = stride;
			fields.push_back(field);
		
			stride += field.attribute.size;
		}
	}
}