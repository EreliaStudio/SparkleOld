#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	void AbstractPipeline::Configuration::_parseInstruction(const ShaderModule::Instruction &p_instruction)
	{
		void (AbstractPipeline::Configuration::*handlerArray[])(const ShaderModule::Instruction &) = {
			&AbstractPipeline::Configuration::_parseVersion,	   // 0b0000000000000001
			&AbstractPipeline::Configuration::_parseStorageBuffer, // 0b0000000000000010
			&AbstractPipeline::Configuration::_parseOutputBuffer,  // 0b0000000000000100
			&AbstractPipeline::Configuration::_parsePushConstant,  // 0b0000000000001000
			&AbstractPipeline::Configuration::_parseUniformBlock,  // 0b0000000000010000
			&AbstractPipeline::Configuration::_parseUniformBlock,  // 0b0000000000100000
			&AbstractPipeline::Configuration::_parseStructure,	   // 0b0000000001000000
			&AbstractPipeline::Configuration::_parseFunction,	   // 0b0000000010000000
			&AbstractPipeline::Configuration::_parseError		   // 0b0000000100000000
		};

		int index = static_cast<int>(std::log2(static_cast<int>(p_instruction.type)));

		(this->*(handlerArray[index]))(p_instruction);
	}

	void AbstractPipeline::Configuration::_parseInstructionSet(const ShaderModule::InstructionSet &p_instructionSet, const int &p_typeMask)
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

	AbstractPipeline::Configuration::Configuration(const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput) : _structureLayout(),
																															_storageBufferLayout(_structureLayout),
																															_outputBufferLayout(_structureLayout),
																															_pushConstantLayout(_structureLayout)
	{
		_parseInstructionSet(p_vertexInput.instructions(), _vertexTypeMask);
		_parseInstructionSet(p_fragmentInput.instructions(), _fragmentTypeMask);
	}

	std::wostream& operator<<(std::wostream& p_out, const AbstractPipeline::Configuration& p_config)
	{
		p_out << "Configuration {" << std::endl;
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

	const AbstractPipeline::Configuration::StorageBufferLayout &AbstractPipeline::Configuration::storageBufferLayout() const
	{
		return (_storageBufferLayout);
	}

	const AbstractPipeline::Configuration::OutputBufferLayout &AbstractPipeline::Configuration::outputBufferLayout() const
	{
		return (_outputBufferLayout);
	}

	const AbstractPipeline::Configuration::PushConstantLayout &AbstractPipeline::Configuration::pushConstantLayout() const
	{
		return (_pushConstantLayout);
	}

	const std::vector<AbstractPipeline::Configuration::UniformBlock> &AbstractPipeline::Configuration::uniformBlocks() const
	{
		return (_uniformBlocks);
	}
}