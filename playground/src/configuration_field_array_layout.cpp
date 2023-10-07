#include "pipeline.hpp"

#include <regex>

AbstractPipeline::Configuration::FieldArrayLayout::Field::Field(const AbstractPipeline::Configuration::Data& p_data, const size_t& p_offset) :
	data(p_data),
	offset(p_offset),
	location(0)
{

}

AbstractPipeline::Configuration::FieldArrayLayout::Field::Field(const AbstractPipeline::Configuration::Data& p_data, const size_t& p_location, const size_t& p_offset) :
	data(p_data),
	offset(p_offset),
	location(p_location)
{

}

AbstractPipeline::Configuration::FieldArrayLayout::FieldArrayLayout(const StructureLayout& p_structureLayout) :
	structureLayout(p_structureLayout)
{
}

const size_t &AbstractPipeline::Configuration::FieldArrayLayout::stride() const
{
	return (_stride);
}

const std::vector<AbstractPipeline::Configuration::FieldArrayLayout::Field> &AbstractPipeline::Configuration::FieldArrayLayout::fields() const
{
	return (_fields);
}
