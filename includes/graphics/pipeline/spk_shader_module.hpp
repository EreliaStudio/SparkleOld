#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace spk
{
    /**
     * @class ShaderModule
     * @brief Represents a shader module containing shader instructions.
     */
	class ShaderModule
	{
	public:
        /**
         * @struct Instruction
         * @brief Represents a single shader instruction.
         */
		struct Instruction
		{
            /// Enumerates types of shader instructions.
			enum class Type
			{
				Version = 0b0000000000000001, /// Represents the version information of the shader.
				StorageBuffer = 0b0000000000000010, /// Represents a shader storage buffer object (SSBO).
				OutputBuffer = 0b0000000000000100, /// Represents an output buffer, where the shader writes its results.
				PushConstant = 0b0000000000001000, /// Represents a push constant, a small, fast constant data storage.
				SamplerUniform = 0b0000000000010000, /// Represents a sampler uniform variable.
				UniformBlock = 0b0000000000100000, /// Represents a uniform block, a set of uniforms grouped together.
				Structure = 0b0000000001000000, /// Represents a custom data structure within the shader.
				Function = 0b0000000010000000, /// Represents a function within the shader.
				Error = 0b0000000100000000 /// Represents an error or invalid instruction.
			};

            /// The type of the instruction.
			Type type;
            /// The shader code associated with the instruction.
			std::string code;

            /**
             * @brief Constructs an Instruction.
             * 
             * @param p_type The type of instruction.
             * @param p_code The shader code for the instruction.
             */
			Instruction(const Type &p_type, const std::string &p_code);

			/// Operator overloads to provide formatted output of the Instruction type.
			friend std::wostream &operator<<(std::wostream &p_os, const Type &p_type);
		};

        /// Type alias for a set of Instructions.
		using InstructionSet = std::vector<Instruction>;

	private:
        /// The name of the shader module.
		std::string _name;
        /// The full shader code.
		std::string _code;
        /// The set of parsed instructions from the shader code.
		InstructionSet _instructions;

        /// Validates the shader code.
		void _checkCodeValidity();
        /// Compacts shader code to a standardized format.
		std::string _compactify(const std::string &p_code);
        /// Determines the type of a given shader instruction.
		Instruction::Type _determineInstructionType(const std::string &p_instruction);
        /// Parses the shader code to extract instructions.
		void _parseInstructions(const std::string &p_code);

	public:
        /**
         * @brief Constructs a ShaderModule from a file.
         * 
         * @param p_filePath The file path to the shader code.
         */
		ShaderModule(const std::filesystem::path &p_filePath);

        /**
         * @brief Constructs a ShaderModule with specified name and code.
         * 
         * @param p_name The name of the shader module.
         * @param p_code The shader code.
         */
		ShaderModule(const std::string &p_name, const std::string &p_code);

        /// Returns the name of the shader module.
		const std::string& name() const;
        /// Returns the shader code.
		const std::string& code() const;
        /// Returns the set of parsed instructions.
		const InstructionSet& instructions() const;
	};
}
