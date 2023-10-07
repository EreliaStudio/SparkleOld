#include "pipeline.hpp"

#include <regex>

void AbstractPipeline::Input::checkCodeValidity()
{
}

AbstractPipeline::Input::Input(const std::filesystem::path &p_filePath) : Input(p_filePath.filename().string(), spk::getFileContentAsString(p_filePath))
{}

AbstractPipeline::Input::Input(const std::string &p_name, const std::string &p_code) :
	name(p_name),
	code(p_code)
{
	checkCodeValidity();
}

AbstractPipeline::Configuration::InstructionSet::Instruction::Instruction(const AbstractPipeline::Configuration::InstructionSet::Instruction::Type &p_type, const std::string &p_code) :
	type(p_type),
	code(p_code)
{
}

std::wostream &operator<<(std::wostream &p_os, const AbstractPipeline::Configuration::InstructionSet::Instruction::Type &p_type)
{
	switch (p_type)
	{
	case AbstractPipeline::Configuration::InstructionSet::Instruction::Type::StorageBuffer:
		p_os << L"StorageBuffer";
		break;
	case AbstractPipeline::Configuration::InstructionSet::Instruction::Type::OutputBuffer:
		p_os << L"OutputBuffer";
		break;
	case AbstractPipeline::Configuration::InstructionSet::Instruction::Type::UniformBlock:
		p_os << L"UniformBlock";
		break;
	case AbstractPipeline::Configuration::InstructionSet::Instruction::Type::SamplerUniform:
		p_os << L"SamplerUniform";
		break;
	case AbstractPipeline::Configuration::InstructionSet::Instruction::Type::PushConstant:
		p_os << L"PushConstant";
		break;
	case AbstractPipeline::Configuration::InstructionSet::Instruction::Type::Function:
		p_os << L"Function";
		break;
	case AbstractPipeline::Configuration::InstructionSet::Instruction::Type::Structure:
		p_os << L"Structure";
		break;
	case AbstractPipeline::Configuration::InstructionSet::Instruction::Type::Version:
		p_os << L"Version";
		break;
	case AbstractPipeline::Configuration::InstructionSet::Instruction::Type::Error:
		p_os << L"Error";
		break;
	default:
		p_os << L"Not defined";
		break;
	}
	return (p_os);
}

std::string AbstractPipeline::Configuration::InstructionSet::compactify(const std::string &p_code)
{
	std::string firstLine = p_code.substr(0, p_code.find('\n') + 1);
	std::string restOfTheCode = p_code.substr(p_code.find('\n') + 1);

	std::vector<std::pair<std::regex, std::string>> regexes = {
		std::make_pair(std::regex("//[^\n]*"), ""),	   // Remove single-line comments
		std::make_pair(std::regex("\n|\r"), " "),	   // Remove newlines and carriage returns
		std::make_pair(std::regex("/\\*.*?\\*/"), ""), // Remove multi-line comments
		std::make_pair(std::regex("\t"), " "),		   // Remove tabs and replace with spaces
		std::make_pair(std::regex(" +"), " "),		   // Remove extra spaces
		std::make_pair(std::regex(" *\\= *"), "="),	   // Remove space around equal
	};

	for (auto &regex : regexes)
	{
		restOfTheCode = std::regex_replace(restOfTheCode, std::get<0>(regex), std::get<1>(regex));
	}

	return (firstLine + restOfTheCode);
}

void AbstractPipeline::Configuration::InstructionSet::parseInstructions(const std::string &p_code)
{
	int braceCount = 0;
	bool requiresSemicolon = false;
	std::string newInstruction;

	for (char c : p_code)
	{
		if (c != '\n' && !(newInstruction.empty() && c == ' '))
			newInstruction += c;

		if (c == '{')
		{
			++braceCount;
		}
		else if (c == '}')
		{
			if (braceCount == 0)
				throw std::runtime_error("Unbalanced braces in GLSL code");
			--braceCount;
		}

		if (newInstruction.size() >= 6 && !requiresSemicolon)
		{
			requiresSemicolon = (newInstruction.compare(0, 6, "layout") == 0 ||
								 newInstruction.compare(0, 6, "struct") == 0 ||
								 newInstruction.compare(0, 7, "uniform") == 0);
		}

		if ((c == ';' || ((c == '}' || c == '\n') && !requiresSemicolon)) && braceCount == 0)
		{
			_instructions.emplace_back(determineInstructionType(newInstruction), newInstruction);

			newInstruction.clear();
			requiresSemicolon = false;
		}
	}
}

AbstractPipeline::Configuration::InstructionSet::Instruction::Type AbstractPipeline::Configuration::InstructionSet::determineInstructionType(const std::string &p_instruction)
{
	std::vector<std::pair<std::regex, Instruction::Type>> rules = {
		{std::regex(R"(#version\s+\d+)"), Instruction::Type::Version},
		{std::regex(R"(layout\(location\s*=\s*\d+\)\s+in)"), Instruction::Type::StorageBuffer},
		{std::regex(R"(layout\(location\s*=\s*\d+\)\s+out)"), Instruction::Type::OutputBuffer},
		{std::regex(R"(layout\(push_constant\)\s+uniform)"), Instruction::Type::PushConstant},
		{std::regex(R"(layout\((?:set\s*=\s*\d+,\s*)?binding\s*=\s*\d+\)\s+uniform\s+(?!sampler))"), Instruction::Type::UniformBlock},
		{std::regex(R"(layout\((?:set\s*=\s*\d+,\s*)?binding\s*=\s*\d+\)\s+uniform\s+sampler)"), Instruction::Type::SamplerUniform},
		{std::regex(R"(struct\s+\w+\s*\{[^\}]+\})"), Instruction::Type::Structure},
		{std::regex(R"(\w+\s+\w+\s*\([^)]*\)\s*\{)"), Instruction::Type::Function}};

	for (auto &rule : rules)
	{
		if (std::regex_search(p_instruction, rule.first))
		{
			return rule.second;
		}
	}
	return Instruction::Type::Error;
}

AbstractPipeline::Configuration::InstructionSet::InstructionSet(const Input &p_input)
{
	parseInstructions(compactify(p_input.code));
}

const std::vector<AbstractPipeline::Configuration::InstructionSet::Instruction> &AbstractPipeline::Configuration::InstructionSet::instructions() const
{
	return (_instructions);
}

AbstractPipeline::Configuration::Data::Data(const Type &p_type, const size_t &p_format, const size_t &p_size) : type(p_type),
																									 format(p_format),
																									 size(p_size)
{
}

AbstractPipeline::Configuration::Data::Data(const size_t &p_size) : type(Type::Structure),
								   format(1),
								   size(p_size)
{
}

AbstractPipeline::Configuration::StructureLayout::StructureLayout()
{
}

void AbstractPipeline::Configuration::StructureLayout::reset()
{
	_structures = {
		{"float", Data(Data::Type::Float, 1, sizeof(float))},
		{"int", Data(Data::Type::Int, 1, sizeof(int))},
		{"uint", Data(Data::Type::UInt, 1, sizeof(unsigned int))},

		{"vec2", Data(Data::Type::Float, 2, sizeof(float))},
		{"ivec2", Data(Data::Type::Int, 2, sizeof(int))},
		{"uvec2", Data(Data::Type::UInt, 2, sizeof(unsigned int))},

		{"vec3", Data(Data::Type::Float, 3, sizeof(float))},
		{"ivec3", Data(Data::Type::Int, 3, sizeof(int))},
		{"uvec3", Data(Data::Type::UInt, 3, sizeof(unsigned int))},

		{"vec4", Data(Data::Type::Float, 4, sizeof(float))},
		{"ivec4", Data(Data::Type::Int, 4, sizeof(int))},
		{"uvec4", Data(Data::Type::UInt, 4, sizeof(unsigned int))},

		{"mat4", Data(Data::Type::Float, 16, sizeof(float))},
	};

	_standaloneStructures = {
		{"sampler1D", Data(Data::Type::Int, 1, sizeof(int))},
		{"sampler2D", Data(Data::Type::Int, 1, sizeof(int))},
		{"sampler3D", Data(Data::Type::Int, 1, sizeof(int))},
		{"samplerCube", Data(Data::Type::Int, 1, sizeof(int))},
	};
}

void AbstractPipeline::Configuration::StructureLayout::treat(const AbstractPipeline::Configuration::InstructionSet::Instruction &p_instruction)
{
}

const std::map<std::string, AbstractPipeline::Configuration::Data> &AbstractPipeline::Configuration::StructureLayout::structures() const
{
	return (_structures);
}

const std::map<std::string, AbstractPipeline::Configuration::Data> &AbstractPipeline::Configuration::StructureLayout::standaloneStructures() const
{
	return (_standaloneStructures);
}

AbstractPipeline::Configuration::ConfigurationLayout::Field::Field(const AbstractPipeline::Configuration::Data& p_data, const size_t& p_offset) :
	data(p_data),
	offset(p_offset),
	location(0)
{

}

AbstractPipeline::Configuration::ConfigurationLayout::Field::Field(const AbstractPipeline::Configuration::Data& p_data, const size_t& p_location, const size_t& p_offset) :
	data(p_data),
	offset(p_offset),
	location(p_location)
{

}

AbstractPipeline::Configuration::ConfigurationLayout::ConfigurationLayout(
	const std::map<std::string, AbstractPipeline::Configuration::Data> &p_structures,
	const std::map<std::string, AbstractPipeline::Configuration::Data> &p_standaloneStructures) :
	structures(p_structures),
	standaloneStructures(p_standaloneStructures)
{
}

AbstractPipeline::Configuration::StorageBufferLayout::StorageBufferLayout(const std::map<std::string, Data> &p_structures, const std::map<std::string, Data> &p_standaloneStructures) : ConfigurationLayout(p_structures, p_standaloneStructures)
{
}

void AbstractPipeline::Configuration::StorageBufferLayout::treat(const AbstractPipeline::Configuration::InstructionSet::Instruction &p_instruction)
{
	spk::cout << "Parsing StorageBuffer instruction [" << spk::to_wstring(p_instruction.code) << "]" << std::endl;
}

const size_t &AbstractPipeline::Configuration::StorageBufferLayout::stride() const
{
	return (_stride);
}

const std::vector<AbstractPipeline::Configuration::StorageBufferLayout::Field> &AbstractPipeline::Configuration::StorageBufferLayout::fields() const
{
	return (_fields);
}

AbstractPipeline::Configuration::PushConstantLayout::PushConstantLayout(
		const std::map<std::string, AbstractPipeline::Configuration::Data> &p_structures,
		const std::map<std::string, AbstractPipeline::Configuration::Data> &p_standaloneStructures) :
	ConfigurationLayout(p_structures, p_standaloneStructures)
{
}

void AbstractPipeline::Configuration::PushConstantLayout::treat(const InstructionSet::Instruction &p_instruction)
{
	spk::cout << "Parsing PushConstant instruction [" << spk::to_wstring(p_instruction.code) << "]" << std::endl;
}

const size_t &AbstractPipeline::Configuration::PushConstantLayout::stride() const
{
	return (_stride);
}

const std::vector<AbstractPipeline::Configuration::PushConstantLayout::Field> &AbstractPipeline::Configuration::PushConstantLayout::fields() const
{
	return (_fields);
}

AbstractPipeline::Configuration::UniformBlockLayout::UniformBlock::Key::Key(size_t p_set, size_t p_binding)
	: binding(p_binding), set(p_set)
{
}

bool AbstractPipeline::Configuration::UniformBlockLayout::UniformBlock::Key::operator<(const AbstractPipeline::Configuration::UniformBlockLayout::UniformBlock::Key &p_other) const
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

AbstractPipeline::Configuration::UniformBlockLayout::UniformBlock::UniformBlock()
{
}

void AbstractPipeline::Configuration::UniformBlockLayout::UniformBlock::treat(const AbstractPipeline::Configuration::InstructionSet::Instruction &p_instruction)
{
	spk::cout << "Parsing UniformBlock instruction [" << spk::to_wstring(p_instruction.code) << "]" << std::endl;
}

const AbstractPipeline::Configuration::UniformBlockLayout::UniformBlock::Key &AbstractPipeline::Configuration::UniformBlockLayout::UniformBlock::key() const
{
	return (_key);
}

const AbstractPipeline::Configuration::UniformBlockLayout::UniformBlock::Mode &AbstractPipeline::Configuration::UniformBlockLayout::UniformBlock::mode() const
{
	return (_mode);
}

const size_t &AbstractPipeline::Configuration::UniformBlockLayout::UniformBlock::stride() const
{
	return (_stride);
}

const std::vector<AbstractPipeline::Configuration::UniformBlockLayout::UniformBlock::Field> &AbstractPipeline::Configuration::UniformBlockLayout::UniformBlock::fields() const
{
	return (_fields);
}

AbstractPipeline::Configuration::UniformBlockLayout::UniformBlockLayout(const std::map<std::string, AbstractPipeline::Configuration::Data> &p_structures, const std::map<std::string, AbstractPipeline::Configuration::Data> &p_standaloneStructures) :
	ConfigurationLayout(p_structures, p_standaloneStructures)
{
}

void AbstractPipeline::Configuration::UniformBlockLayout::treat(const AbstractPipeline::Configuration::InstructionSet::Instruction &p_instruction)
{
	UniformBlock newUniformBlock;

	newUniformBlock.treat(p_instruction);

	if (uniformBlocks.contains(newUniformBlock.key()) == true)
	{
		const auto &oldBlock = uniformBlocks.at(newUniformBlock.key());
		if (oldBlock.stride() != newUniformBlock.stride())
		{
			spk::throwException(L"Instruction [" + spk::to_wstring(p_instruction.code) + L"}\n - Uniform block [set = " + std::to_wstring(newUniformBlock.key().set) + L" / binding = " + std::to_wstring(newUniformBlock.key().binding) + L"] already exist and have a different composition");
		}
	}
}

const std::vector<AbstractPipeline::Configuration::UniformBlockLayout::UniformBlock::Key> &AbstractPipeline::Configuration::UniformBlockLayout::subscribedUniformBlocks() const
{
	return (_subscribedUniformBlocks);
}

AbstractPipeline::Configuration::OutputBufferLayout::Field::Field(const Data &p_data, const size_t &p_location, const size_t &p_offset) :
	data(p_data),
	location(p_location),
	offset(p_offset)
{
}

AbstractPipeline::Configuration::OutputBufferLayout::OutputBufferLayout(const std::map<std::string, Data> &p_structures, const std::map<std::string, Data> &p_standaloneStructures) : ConfigurationLayout(p_structures, p_standaloneStructures)
{
}

void AbstractPipeline::Configuration::OutputBufferLayout::treat(const AbstractPipeline::Configuration::InstructionSet::Instruction &p_instruction)
{
	spk::cout << "Parsing OutputBuffer instruction [" << spk::to_wstring(p_instruction.code) << "]" << std::endl;
}

const size_t &AbstractPipeline::Configuration::OutputBufferLayout::stride() const
{
	return (_stride);
}

const std::vector<AbstractPipeline::Configuration::OutputBufferLayout::Field> &AbstractPipeline::Configuration::OutputBufferLayout::fields() const
{
	return (_fields);
}

void AbstractPipeline::Configuration::_parseVersion(const InstructionSet::Instruction &p_instruction)
{
	// Do nothing
}

void AbstractPipeline::Configuration::_parseStorageBuffer(const InstructionSet::Instruction &p_instruction)
{
	_storageBufferLayout.treat(p_instruction);
}

void AbstractPipeline::Configuration::_parseOutputBuffer(const InstructionSet::Instruction &p_instruction)
{
	_outputBufferLayout.treat(p_instruction);
}

void AbstractPipeline::Configuration::_parsePushConstant(const InstructionSet::Instruction &p_instruction)
{
	_pushConstantLayout.treat(p_instruction);
}

void AbstractPipeline::Configuration::_parseUniformBlock(const InstructionSet::Instruction &p_instruction)
{
	_uniformBlockLayout.treat(p_instruction);
}

void AbstractPipeline::Configuration::_parseStructure(const InstructionSet::Instruction &p_instruction)
{
	_structureLayout.treat(p_instruction);
}

void AbstractPipeline::Configuration::_parseFunction(const InstructionSet::Instruction &p_instruction)
{
	// Do nothing
}

void AbstractPipeline::Configuration::_parseError(const InstructionSet::Instruction &p_instruction)
{
	spk::throwException(L"A non-recognized instruction detected :\n- [" + spk::to_wstring(p_instruction.code) + L"]");
}

void AbstractPipeline::Configuration::_parseInstruction(const InstructionSet::Instruction &p_instruction)
{
	// Declare an array of member function pointers, make sure the order is correct.
	void (AbstractPipeline::Configuration::*handlerArray[])(const InstructionSet::Instruction &) = {
		&AbstractPipeline::Configuration::_parseVersion,		 // 0b0000000000000001
		&AbstractPipeline::Configuration::_parseStorageBuffer, // 0b0000000000000010
		&AbstractPipeline::Configuration::_parseOutputBuffer,	 // 0b0000000000000100
		&AbstractPipeline::Configuration::_parsePushConstant,	 // 0b0000000000001000
		&AbstractPipeline::Configuration::_parseUniformBlock,	 // 0b0000000000010000
		&AbstractPipeline::Configuration::_parseUniformBlock,	 // 0b0000000000100000
		&AbstractPipeline::Configuration::_parseStructure,	 // 0b0000000001000000
		&AbstractPipeline::Configuration::_parseFunction,		 // 0b0000000010000000
		&AbstractPipeline::Configuration::_parseError			 // 0b0000000100000000
	};

	// Calculate the index based on the bitmask
	int index = static_cast<int>(std::log2(static_cast<int>(p_instruction.type)));

	// Call the function
	(this->*(handlerArray[index]))(p_instruction);
}

void AbstractPipeline::Configuration::_parseInstructionSet(const InstructionSet &p_instructionSet, const int &p_typeMask)
{
	_structureLayout.reset();
	for (const auto &instruction : p_instructionSet.instructions())
	{
		if (p_typeMask & static_cast<int>(instruction.type))
		{
			_parseInstruction(instruction);
		}
	}
}

AbstractPipeline::Configuration::Configuration(const Input &p_vertexInput, const Input &p_fragmentInput) :
	_structureLayout(),
	_storageBufferLayout(_structureLayout.structures(), _structureLayout.standaloneStructures()),
	_outputBufferLayout(_structureLayout.structures(), _structureLayout.standaloneStructures()),
	_pushConstantLayout(_structureLayout.structures(), _structureLayout.standaloneStructures()),
	_uniformBlockLayout(_structureLayout.structures(), _structureLayout.standaloneStructures())
{
	_parseInstructionSet(InstructionSet(p_vertexInput), _vertexTypeMask);
	_parseInstructionSet(InstructionSet(p_fragmentInput), _fragmentTypeMask);
}

void AbstractPipeline::_loadPipeline(const Input &p_vertexInput, const Input &p_fragmentInput)
{
	_loadProgram(Configuration(p_vertexInput, p_fragmentInput), p_vertexInput, p_fragmentInput);
}
