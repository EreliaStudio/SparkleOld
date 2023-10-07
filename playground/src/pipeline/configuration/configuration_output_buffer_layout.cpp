#include "pipeline/pipeline.hpp"

AbstractPipeline::Configuration::OutputBufferLayout::OutputBufferLayout(const StructureLayout& p_structureLayout) :
	FieldArrayLayout(p_structureLayout)
{
}

void AbstractPipeline::Configuration::OutputBufferLayout::treat(const ShaderModule::Instruction &p_instruction)
{
	spk::cout << "Parsing OutputBuffer instruction [" << spk::to_wstring(p_instruction.code) << "]" << std::endl;
}