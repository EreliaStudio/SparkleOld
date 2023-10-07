#include "pipeline/pipeline.hpp"

AbstractPipeline::Configuration::FieldArrayLayout::Field::Field(const AbstractPipeline::Configuration::Data& p_data) :
	data(p_data),
	offset(0),
	location(0)
{

}

AbstractPipeline::Configuration::FieldArrayLayout::Field::Field(const AbstractPipeline::Configuration::Data& p_data, const size_t& p_location) :
	data(p_data),
	offset(0),
	location(p_location)
{

}

AbstractPipeline::Configuration::FieldArrayLayout::FieldArrayLayout(const StructureLayout& p_structureLayout) :
	structureLayout(p_structureLayout)
{

}

void AbstractPipeline::Configuration::FieldArrayLayout::insert(const Data& p_data, const size_t& p_location)
{
	Field newField = Field(p_data, p_location);

	newField.offset = stride();

	_fields.push_back(newField);
	_stride += newField.data.format * newField.data.size;
}

const size_t &AbstractPipeline::Configuration::FieldArrayLayout::stride() const
{
	return (_stride);
}

const std::vector<AbstractPipeline::Configuration::FieldArrayLayout::Field> &AbstractPipeline::Configuration::FieldArrayLayout::fields() const
{
	return (_fields);
}
