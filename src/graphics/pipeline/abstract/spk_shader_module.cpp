#include "graphics/pipeline/spk_shader_module.hpp"

#include "spk_basic_functions.hpp"

#include <regex>

namespace spk
{
	void ShaderModule::_checkCodeValidity()
	{
	}

	ShaderModule::ShaderModule(const std::filesystem::path &p_filePath) :
		ShaderModule::ShaderModule(p_filePath.filename().string(), spk::getFileContentAsString(p_filePath))
	{}

	ShaderModule::ShaderModule(const std::string &p_name, const std::string &p_code) :
		_name(p_name),
		_code(p_code)
	{
		_checkCodeValidity();
		_parseInstructions(_compactify(p_code));
	}

	const std::string& ShaderModule::name() const
	{
		return (_name);
	}

	const std::string& ShaderModule::code() const
	{
		return (_code);
	}

	const ShaderModule::InstructionSet& ShaderModule::instructions() const
	{
		return (_instructions);
	}

	ShaderModule::Instruction::Instruction(const ShaderModule::Instruction::Type &p_type, const std::string &p_code) :
		type(p_type),
		code(p_code)
	{
	}

	std::wostream &operator<<(std::wostream &p_os, const ShaderModule::Instruction::Type &p_type)
	{
		switch (p_type)
		{
		case ShaderModule::Instruction::Type::StorageBuffer:
			p_os << L"StorageBuffer";
			break;
		case ShaderModule::Instruction::Type::OutputBuffer:
			p_os << L"OutputBuffer";
			break;
		case ShaderModule::Instruction::Type::UniformBlock:
			p_os << L"UniformBlock";
			break;
		case ShaderModule::Instruction::Type::SamplerUniform:
			p_os << L"SamplerUniform";
			break;
		case ShaderModule::Instruction::Type::PushConstant:
			p_os << L"PushConstant";
			break;
		case ShaderModule::Instruction::Type::Function:
			p_os << L"Function";
			break;
		case ShaderModule::Instruction::Type::Structure:
			p_os << L"Structure";
			break;
		case ShaderModule::Instruction::Type::Version:
			p_os << L"Version";
			break;
		case ShaderModule::Instruction::Type::Error:
			p_os << L"Error";
			break;
		default:
			p_os << L"Not defined";
			break;
		}
		return (p_os);
	}

	std::string ShaderModule::_compactify(const std::string &p_code)
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

	void ShaderModule::_parseInstructions(const std::string &p_code)
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
				_instructions.emplace_back(_determineInstructionType(newInstruction), newInstruction);

				newInstruction.clear();
				requiresSemicolon = false;
			}
		}
	}

	ShaderModule::Instruction::Type ShaderModule::_determineInstructionType(const std::string &p_instruction)
	{
		std::vector<std::pair<std::regex, Instruction::Type>> rules = {
			{std::regex(R"(#version\s+\d+)"), Instruction::Type::Version},
			{std::regex(R"(layout\(location\s*=\s*\d+\)\s+in)"), Instruction::Type::StorageBuffer},
			{std::regex(R"(layout\(location\s*=\s*\d+\)\s+out)"), Instruction::Type::OutputBuffer},
			{std::regex(R"(layout\(push_constant\)\s+uniform)"), Instruction::Type::PushConstant},
			{std::regex(R"(layout\s*\((?:set\s*=\s*\d+,\s*)?binding\s*=\s*\d+\)\s+uniform\s+\w+\s*\{)"), Instruction::Type::UniformBlock},
			{std::regex(R"(layout\s*\((?:set\s*=\s*\d+,\s*)?binding\s*=\s*\d+\)\s+uniform\s+\w+\s+\w+\s*;)"), Instruction::Type::SamplerUniform},
			{std::regex(R"(struct\s+\w+\s*\{[^\}]+\})"), Instruction::Type::Structure},
			{std::regex(R"(\w+\s+\w+\s*\([^)]*\)\s*\{)"), Instruction::Type::Function}
		};

		for (auto &rule : rules)
		{
			if (std::regex_search(p_instruction, rule.first))
			{
				return rule.second;
			}
		}
		return Instruction::Type::Error;
	}
}