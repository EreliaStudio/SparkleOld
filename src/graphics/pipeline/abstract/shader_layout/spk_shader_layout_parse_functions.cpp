#include "graphics/pipeline/spk_shader_layout.hpp"

#include "spk_basic_functions.hpp"

namespace spk
{
	void ShaderLayout::_parseVersion(const ShaderModule::Instruction &p_instruction)
	{
		// Do nothing
	}

	void ShaderLayout::_parseStorageBuffer(const ShaderModule::Instruction &p_instruction)
	{
		_storageBufferLayout.treat(p_instruction);
	}

	void ShaderLayout::_parseOutputBuffer(const ShaderModule::Instruction &p_instruction)
	{
		_outputBufferLayout.treat(p_instruction);
	}

	void ShaderLayout::_parsePushConstant(const ShaderModule::Instruction &p_instruction)
	{
		_pushConstantsLayout.treat(p_instruction);
	}

	void ShaderLayout::_parseUniformBlock(const ShaderModule::Instruction &p_instruction)
	{
		UniformBlockLayout newUniformBlock(_structureLayout);

		newUniformBlock.treat(p_instruction);

		_uniformBlocksLayout.push_back(newUniformBlock);
	}

	void ShaderLayout::_parseStructure(const ShaderModule::Instruction &p_instruction)
	{
		_structureLayout.treat(p_instruction);
	}

	void ShaderLayout::_parseFunction(const ShaderModule::Instruction &p_instruction)
	{
		// Do nothing
	}

	void ShaderLayout::_parseError(const ShaderModule::Instruction &p_instruction)
	{
		spk::throwException(L"A non-recognized instruction detected :\n- [" + spk::to_wstring(p_instruction.code) + L"]");
	}
}