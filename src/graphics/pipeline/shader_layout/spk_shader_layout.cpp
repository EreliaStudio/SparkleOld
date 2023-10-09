#include "graphics/pipeline/spk_shader_layout.hpp"

namespace spk
{
	void ShaderLayout::_parseInstruction(const ShaderModule::Instruction &p_instruction)
	{
		void (ShaderLayout::*handlerArray[])(const ShaderModule::Instruction &) = {
			&ShaderLayout::_parseVersion,	   // 0b0000000000000001
			&ShaderLayout::_parseStorageBuffer, // 0b0000000000000010
			&ShaderLayout::_parseOutputBuffer,  // 0b0000000000000100
			&ShaderLayout::_parsePushConstant,  // 0b0000000000001000
			&ShaderLayout::_parseUniformBlock,  // 0b0000000000010000
			&ShaderLayout::_parseUniformBlock,  // 0b0000000000100000
			&ShaderLayout::_parseStructure,	   // 0b0000000001000000
			&ShaderLayout::_parseFunction,	   // 0b0000000010000000
			&ShaderLayout::_parseError		   // 0b0000000100000000
		};

		int index = static_cast<int>(std::log2(static_cast<int>(p_instruction.type)));

		(this->*(handlerArray[index]))(p_instruction);
	}

	void ShaderLayout::_parseInstructionSet(const ShaderModule::InstructionSet &p_instructionSet, const int &p_typeMask)
	{
		_structureLayout.reset();

		for (const auto &instruction : p_instructionSet)
		{
			if ((p_typeMask & static_cast<int>(instruction.type)) == static_cast<int>(instruction.type))
			{
				_parseInstruction(instruction);
			}
		}
	}

	ShaderLayout::ShaderLayout(const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput) :
		_structureLayout(),
		_storageBufferLayout(_structureLayout),
		_outputBufferLayout(_structureLayout),
		_pushConstantLayout(_structureLayout)
	{
		_parseInstructionSet(p_vertexInput.instructions(), _vertexTypeMask);
		_parseInstructionSet(p_fragmentInput.instructions(), _fragmentTypeMask);
	}

	std::wostream& operator<<(std::wostream& p_out, const ShaderLayout& p_config)
	{
		p_out << "Configuration {" << std::endl;
		p_out << "\tStructure: \n" << p_config.structureLayout() << std::endl;
		p_out << "\tStorageBufferLayout: \n" << p_config.storageBufferLayout() << std::endl;
		p_out << "\tOutputBufferLayout: \n" << p_config.outputBufferLayout() << std::endl;
		p_out << "\tPushConstantLayout: \n" << p_config.pushConstantLayout() << std::endl;
		
		p_out << "\tUniformBlocks: [" << std::endl;
		for (const auto& block : p_config.uniformBlocks())
		{
			p_out << block << std::endl;
		}
		p_out << "\t]\n";
		
		p_out << "}";
		return p_out;
	}

	const ShaderLayout::StructureLayout &ShaderLayout::structureLayout() const
	{
		return (_structureLayout);
	}

	const ShaderLayout::StorageBufferLayout &ShaderLayout::storageBufferLayout() const
	{
		return (_storageBufferLayout);
	}

	const ShaderLayout::OutputBufferLayout &ShaderLayout::outputBufferLayout() const
	{
		return (_outputBufferLayout);
	}

	const ShaderLayout::PushConstantLayout &ShaderLayout::pushConstantLayout() const
	{
		return (_pushConstantLayout);
	}

	const std::vector<ShaderLayout::UniformBlock> &ShaderLayout::uniformBlocks() const
	{
		return (_uniformBlocks);
	}
}