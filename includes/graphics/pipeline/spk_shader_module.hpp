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
            /**
             * @enum Type
             * @brief Enumerates types of shader instructions.
             */
			enum class Type
			{
				Version, ///< Represents the version information of the shader.
				StorageBuffer, ///< Represents a shader storage buffer object (SSBO).
				OutputBuffer, ///< Represents an output buffer, where the shader writes its results.
				PushConstant, ///< Represents a push constant, a small, fast constant data storage.
				SamplerUniform, ///< Represents a sampler uniform variable.
				UniformBlock, ///< Represents a uniform block, a set of uniforms grouped together.
				Structure, ///< Represents a custom data structure within the shader.
				Function, ///< Represents a function within the shader.
				Error ///< Represents an error or invalid instruction.
			};

            /**
             * @brief The type of the instruction.
             */
			Type type;

            /**
             * @brief The shader code associated with the instruction.
             */
			std::string code;

            /**
             * @brief Constructs an Instruction.
             * 
             * @param p_type The type of instruction.
             * @param p_code The shader code for the instruction.
             */
			Instruction(const Type &p_type, const std::string &p_code);

            /**
             * @brief Operator overload for formatted output of Instruction::Type.
             * 
             * @param p_os The wide output stream where the formatted text will be written to.
             * @param p_type The Instruction::Type object to be formatted and outputted.
             * @return Reference to the output stream.
             */
			friend std::wostream &operator<<(std::wostream &p_os, const Type &p_type);
		};

        /**
         * @typedef InstructionSet
         * @brief Type alias for a set of Instructions.
         */
		using InstructionSet = std::vector<Instruction>;

	private:
        /**
         * @brief The name of the shader module.
         */
		std::string _name;

        /**
         * @brief The full shader code.
         */
		std::string _code;

        /**
         * @brief The set of parsed instructions from the shader code.
         */
		InstructionSet _instructions;

        /**
         * @brief Validates the shader code.
         */
		void _checkCodeValidity();

        /**
         * @brief Compacts shader code to a standardized format.
         * 
         * @param p_code The shader code to be compactified.
         * @return Compactified shader code.
         */
		std::string _compactify(const std::string &p_code);

        /**
         * @brief Determines the type of a given shader instruction.
         * 
         * @param p_instruction The shader instruction to be analyzed.
         * @return The determined instruction type.
         */
		Instruction::Type _determineInstructionType(const std::string &p_instruction);

        /**
         * @brief Parses the shader code to extract instructions.
         * 
         * @param p_code The shader code to be parsed.
         */
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

        /**
         * @brief Returns the name of the shader module.
         * 
         * @return The name of the shader module.
         */
		const std::string& name() const;

        /**
         * @brief Returns the shader code.
         * 
         * @return The shader code.
         */
		const std::string& code() const;

        /**
         * @brief Returns the set of parsed instructions.
         * 
         * @return The set of parsed instructions.
         */
		const InstructionSet& instructions() const;
	};
}
