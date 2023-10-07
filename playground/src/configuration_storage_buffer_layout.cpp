#include "pipeline.hpp"

AbstractPipeline::Configuration::StorageBufferLayout::StorageBufferLayout(const StructureLayout& p_structureLayout) :
	FieldArrayLayout(p_structureLayout)
{
}

void AbstractPipeline::Configuration::StorageBufferLayout::treat(const ShaderModule::Instruction &p_instruction)
{
	spk::cout << "Parsing StorageBuffer instruction [" << spk::to_wstring(p_instruction.code) << "]" << std::endl;
}