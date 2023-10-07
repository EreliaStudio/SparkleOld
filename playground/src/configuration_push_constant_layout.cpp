#include "pipeline.hpp"

AbstractPipeline::Configuration::PushConstantLayout::PushConstantLayout(const StructureLayout& p_structureLayout) :
	FieldArrayLayout(p_structureLayout)
{
}

void AbstractPipeline::Configuration::PushConstantLayout::treat(const ShaderModule::Instruction &p_instruction)
{
	spk::cout << "Parsing PushConstant instruction [" << spk::to_wstring(p_instruction.code) << "]" << std::endl;
}