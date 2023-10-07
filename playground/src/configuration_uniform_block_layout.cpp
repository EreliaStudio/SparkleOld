#include "pipeline.hpp"

AbstractPipeline::Configuration::UniformBlockLayout::Key::Key(size_t p_set, size_t p_binding)
	: binding(p_binding), set(p_set)
{
}

bool AbstractPipeline::Configuration::UniformBlockLayout::Key::operator<(const AbstractPipeline::Configuration::UniformBlockLayout::Key &p_other) const
{
	if (binding < p_other.binding)
	{
		return true;
	}
	else if (binding == p_other.binding)
	{
		return set < p_other.set;
	}
	return false;
}

AbstractPipeline::Configuration::UniformBlockLayout::UniformBlockLayout(const StructureLayout& p_structureLayout) :
	FieldArrayLayout(p_structureLayout)
{
}

void AbstractPipeline::Configuration::UniformBlockLayout::treat(const ShaderModule::Instruction &p_instruction)
{
	spk::cout << "Parsing UniformBlockLayout instruction [" << spk::to_wstring(p_instruction.code) << "]" << std::endl;
}

const AbstractPipeline::Configuration::UniformBlockLayout::Key &AbstractPipeline::Configuration::UniformBlockLayout::key() const
{
	return (_key);
}

const AbstractPipeline::Configuration::UniformBlockLayout::Mode &AbstractPipeline::Configuration::UniformBlockLayout::mode() const
{
	return (_mode);
}
