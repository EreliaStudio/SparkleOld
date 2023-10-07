#include "pipeline.hpp"

#include <regex>

void AbstractPipeline::Configuration::_parseVersion(const ShaderModule::Instruction &p_instruction)
{
	// Do nothing
}

void AbstractPipeline::Configuration::_parseStorageBuffer(const ShaderModule::Instruction &p_instruction)
{
	_storageBufferLayout.treat(p_instruction);
}

void AbstractPipeline::Configuration::_parseOutputBuffer(const ShaderModule::Instruction &p_instruction)
{
	_outputBufferLayout.treat(p_instruction);
}

void AbstractPipeline::Configuration::_parsePushConstant(const ShaderModule::Instruction &p_instruction)
{
	_pushConstantLayout.treat(p_instruction);
}

void AbstractPipeline::Configuration::_parseUniformBlockLayout(const ShaderModule::Instruction &p_instruction)
{
	UniformBlockLayout newUniformBlockLayout(_structureLayout);

	newUniformBlockLayout.treat(p_instruction);

	_uniformBlocks.push_back(newUniformBlockLayout);
}

void AbstractPipeline::Configuration::_parseStructure(const ShaderModule::Instruction &p_instruction)
{
	_structureLayout.treat(p_instruction);
}

void AbstractPipeline::Configuration::_parseFunction(const ShaderModule::Instruction &p_instruction)
{
	// Do nothing
}

void AbstractPipeline::Configuration::_parseError(const ShaderModule::Instruction &p_instruction)
{
	spk::throwException(L"A non-recognized instruction detected :\n- [" + spk::to_wstring(p_instruction.code) + L"]");
}
