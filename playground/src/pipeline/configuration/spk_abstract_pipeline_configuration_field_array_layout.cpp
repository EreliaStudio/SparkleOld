#include "pipeline/pipeline.hpp"

AbstractPipeline::Configuration::FieldArrayLayout::FieldArrayLayout(const StructureLayout &p_structureLayout) : structureLayout(p_structureLayout)
{
}

void AbstractPipeline::Configuration::FieldArrayLayout::insert(const std::string &p_name, const Data &p_data, const size_t &p_location)
{
	Field newField;

	newField.name = p_name;
	newField.data = p_data;
	newField.location = p_location;
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
