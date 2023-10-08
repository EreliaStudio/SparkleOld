#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace spk
{
	class ShaderModule
	{
	public:
		struct Instruction
		{
			enum class Type
			{
				Version = 0b0000000000000001,
				StorageBuffer = 0b0000000000000010,
				OutputBuffer = 0b0000000000000100,
				PushConstant = 0b0000000000001000,
				SingleUniform = 0b0000000000010000,
				UniformBlock = 0b0000000000100000,
				Structure = 0b0000000001000000,
				Function = 0b0000000010000000,
				Error = 0b0000000100000000
			};

			Type type;
			std::string code;

			Instruction(const Type &p_type, const std::string &p_code);
			friend std::wostream &operator<<(std::wostream &p_os, const Type &p_type);
		};
		using InstructionSet = std::vector<Instruction>;

	private:
		std::string _name;
		std::string _code;
		InstructionSet _instructions;

		void _checkCodeValidity();
		std::string _compactify(const std::string &p_code);
		Instruction::Type _determineInstructionType(const std::string &p_instruction);
		void _parseInstructions(const std::string &p_code);

	public:
		ShaderModule(const std::filesystem::path &p_filePath);
		ShaderModule(const std::string &p_name, const std::string &p_code);

		const std::string& name() const;
		const std::string& code() const;
		const InstructionSet& instructions() const;
	};
}