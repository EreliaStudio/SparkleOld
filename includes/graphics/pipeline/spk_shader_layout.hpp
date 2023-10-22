#pragma once

#include "graphics/pipeline/spk_shader_module.hpp"
#include <map>

namespace spk
{
	/**
	 * @class ShaderLayout
	 * @brief Handles the layout of a Shader, including its different modules and layouts for various buffer types.
	 * 
	 * This class aggregates multiple ShaderModule objects and various layouts to fully describe a shader.
	 * It also includes parsing functionalities to understand shader instructions.
	 */
	class ShaderLayout
	{
	public:
		/**
		 * @brief Struct to represent various data types and their attributes.
		 * 
		 * The Data structure is used to store the type, format, and size of a data type.
		 * This can be used for uniform variables, attributes, and other shader-related data.
		 */
		struct Data
		{
			/**
			 * @brief Enumeration for the types of data.
			 * 
			 * This enum represents the types of data that can be handled.
			 */
			enum class Type
			{
				Int,        ///< Integer type
				UInt,       ///< Unsigned Integer type
				Float,      ///< Floating-point type
				Structure,  ///< Structured type
				Error       ///< Error or unknown type
			};

			Type type;      ///< The actual type of the data.
			size_t format;  ///< The format of the data (e.g., how many components like vec3 or vec4).
			size_t size;    ///< The size of the data in bytes.

			/**
			 * @brief Constructor to initialize all attributes.
			 * 
			 * @param p_type The type of the data (default is Error).
			 * @param p_format The format of the data (default is 0).
			 * @param p_size The size of the data in bytes (default is 0).
			 */
			Data(const Type& p_type = Type::Error, const size_t& p_format = 0, const size_t& p_size = 0);

			/**
			 * @brief Overloaded constructor to initialize only the size.
			 * 
			 * @param p_size The size of the data in bytes.
			 */
			Data(const size_t& p_size);

			/**
			 * @brief Overloaded output stream operator for Type enum.
			 * 
			 * @param p_out The output stream.
			 * @param p_data The data type to output.
			 * @return The output stream.
			 */
			friend std::wostream& operator<<(std::wostream& p_out, const Type& p_data);

			/**
			 * @brief Overloaded output stream operator for Data struct.
			 * 
			 * @param p_out The output stream.
			 * @param p_data The Data struct to output.
			 * @return The output stream.
			 */
			friend std::wostream& operator<<(std::wostream& p_out, const Data& p_data);
		};

		/**
		 * @brief Class to handle structure layout configurations.
		 * 
		 * The StructureLayout class is responsible for managing the structure
		 * layouts within shader programs. It keeps track of various uniform and
		 * attribute structures and their corresponding data types.
		 */
		class StructureLayout
		{
		private:
			/**
			 * @brief Map to hold the structures and their data.
			 * 
			 * Key: Name of the structure
			 * Value: Data associated with the structure
			 */
			std::map<std::string, Data> _structures;

			/**
			 * @brief Map to hold the single uniform structures and their data.
			 * 
			 * Key: Name of the single uniform structure
			 * Value: Data associated with the single uniform structure
			 */
			std::map<std::string, Data> _SamplerUniformStructures;

			/**
			 * @brief String representing the accepted single uniform types.
			 */
			std::wstring _acceptedSamplerUniformTypeString;

		public:
			/**
			 * @brief Default constructor for StructureLayout.
			 */
			StructureLayout();

			/**
			 * @brief Resets the internal state of the StructureLayout.
			 */
			void reset();

			/**
			 * @brief Treats an instruction and updates internal state accordingly.
			 * 
			 * @param p_instruction The instruction to be treated.
			 */
			void treat(const ShaderModule::Instruction &p_instruction);

			/**
			 * @brief Gets the accepted single uniform type string.
			 * 
			 * @return The accepted single uniform type string.
			 */
			const std::wstring& acceptedSamplerUniformTypeString() const;

			/**
			 * @brief Gets the map of structures.
			 * 
			 * @return The map of structures.
			 */
			const std::map<std::string, Data>& structures() const;

			/**
			 * @brief Gets the map of single uniform structures.
			 * 
			 * @return The map of single uniform structures.
			 */
			const std::map<std::string, Data>& SamplerUniformStructures() const;

			/**
			 * @brief Overloaded output stream operator for StructureLayout.
			 * 
			 * @param p_out The output stream.
			 * @param p_layout The StructureLayout to output.
			 * @return The output stream.
			 */
			friend std::wostream& operator<<(std::wostream& p_out, const StructureLayout& p_layout);
		};

		/**
		 * @brief Class to manage the layout of a field array.
		 * 
		 * The FieldArrayLayout class is responsible for managing and storing the layout
		 * of field arrays used in the shader. It holds the stride and fields and provides
		 * methods for inserting and treating instructions.
		 */
		class FieldArrayLayout
		{
		public:
			/**
			 * @brief Structure to represent an individual field.
			 */
			struct Field
			{
				std::string name; ///< The name of the field.
				Data data; ///< The data associated with the field.
				int location = -1; ///< The location index of the field.
				size_t offset; ///< The memory offset of the field.

				/**
				 * @brief Overloaded output stream operator for Field.
				 * 
				 * @param p_out The output stream.
				 * @param p_layout The Field to output.
				 * @return The output stream.
				 */
				friend std::wostream& operator<<(std::wostream& p_out, const Field& p_layout);
			};

		private:
			/**
			 * @brief The stride size of the field array.
			 */
			size_t _stride;

			/**
			 * @brief Vector to store the fields.
			 */
			std::vector<Field> _fields;

		protected:
			/**
			 * @brief Reference to the associated StructureLayout.
			 */
			const StructureLayout& structureLayout;

		public:
			/**
			 * @brief Constructor for FieldArrayLayout.
			 * 
			 * @param p_structureLayout Reference to the associated StructureLayout.
			 */
			FieldArrayLayout(const StructureLayout& p_structureLayout);

			/**
			 * @brief Inserts a new field into the layout.
			 * 
			 * @param p_name The name of the field.
			 * @param p_data The data associated with the field.
			 * @param p_location The location index of the field (default is 0).
			 */
			void insert(const std::string& p_name, const Data& p_data, const size_t& p_location = 0);

			/**
			 * @brief Treats an instruction and updates the layout accordingly.
			 * 
			 * This is a pure virtual function and should be implemented by derived classes.
			 * 
			 * @param p_instruction The instruction to be treated.
			 */
			virtual void treat(const ShaderModule::Instruction &p_instruction) = 0;

			/**
			 * @brief Gets the stride size of the field array.
			 * 
			 * @return The stride size.
			 */
			const size_t& stride() const;

			/**
			 * @brief Gets the vector of fields.
			 * 
			 * @return The vector of fields.
			 */
			const std::vector<Field>& fields() const;

			/**
			 * @brief Overloaded output stream operator for FieldArrayLayout.
			 * 
			 * @param p_out The output stream.
			 * @param p_layout The FieldArrayLayout to output.
			 * @return The output stream.
			 */
			friend std::wostream& operator<<(std::wostream& p_out, const FieldArrayLayout& p_layout);
		};

		/**
		 * @brief Class to manage the layout of storage buffers.
		 * 
		 * StorageBufferLayout is responsible for managing the layout of storage
		 * buffers used in the shader. It extends FieldArrayLayout and overrides
		 * the `treat` method for specific behavior.
		 */
		class StorageBufferLayout : public FieldArrayLayout
		{
		public:
			/**
			 * @brief Constructor for StorageBufferLayout.
			 * 
			 * Initializes the StorageBufferLayout using the associated StructureLayout.
			 * 
			 * @param p_structureLayout Reference to the associated StructureLayout.
			 */
			StorageBufferLayout(const StructureLayout& p_structureLayout);

			/**
			 * @brief Treats an instruction and updates the storage buffer layout.
			 * 
			 * Overridden function that treats a shader instruction and updates
			 * the storage buffer layout accordingly.
			 * 
			 * @param p_instruction The shader instruction to be treated.
			 */
			void treat(const ShaderModule::Instruction &p_instruction);
		};

		/**
		 * @brief Class to manage the layout of output buffers.
		 * 
		 * OutputBufferLayout is responsible for managing the layout of output
		 * buffers used in the shader. It extends FieldArrayLayout and overrides
		 * the `treat` method for specific behavior.
		 */
		class OutputBufferLayout : public FieldArrayLayout
		{
		public:
			/**
			 * @brief Constructor for OutputBufferLayout.
			 * 
			 * Initializes the OutputBufferLayout using the associated StructureLayout.
			 * 
			 * @param p_structureLayout Reference to the associated StructureLayout.
			 */
			OutputBufferLayout(const StructureLayout& p_structureLayout);

			/**
			 * @brief Treats an instruction and updates the output buffer layout.
			 * 
			 * Overridden function that treats a shader instruction and updates
			 * the output buffer layout accordingly.
			 * 
			 * @param p_instruction The shader instruction to be treated.
			 */
			void treat(const ShaderModule::Instruction &p_instruction);
		};

		/**
		 * @brief Class to manage the layout of push constants.
		 *
		 * PushConstantsLayout is responsible for managing the layout of push constants 
		 * in the shader. It extends the FieldArrayLayout and specializes the `treat` method 
		 * to handle push constant specific behavior.
		 */
		class PushConstantsLayout : public FieldArrayLayout
		{
		private:
			std::wstring _type;  ///< The type of the push constants.
			std::wstring _name;  ///< The name of the push constant block.

		public:
			/**
			 * @brief Constructor for PushConstantsLayout.
			 * 
			 * Initializes the PushConstantsLayout using the associated StructureLayout.
			 *
			 * @param p_structureLayout Reference to the associated StructureLayout.
			 */
			PushConstantsLayout(const StructureLayout& p_structureLayout);

			/**
			 * @brief Treats an instruction and updates the push constants layout.
			 * 
			 * Overridden function that treats a shader instruction and updates
			 * the push constants layout accordingly.
			 *
			 * @param p_instruction The shader instruction to be treated.
			 */
			void treat(const ShaderModule::Instruction &p_instruction);

			/**
			 * @brief Retrieves the type of the push constants.
			 * 
			 * Accessor function to get the type of the push constants.
			 *
			 * @return The type of the push constants as a wide string.
			 */
			const std::wstring& type() const;

			/**
			 * @brief Retrieves the name of the push constants block.
			 * 
			 * Accessor function to get the name of the push constants block.
			 *
			 * @return The name of the push constants block as a wide string.
			 */
			const std::wstring& name() const;

			/**
			 * @brief Operator overload for formatted output of PushConstantsLayout.
			 *
			 * @param p_out The wide output stream where the formatted text will be written to.
			 * @param p_layout The PushConstantsLayout object to be formatted and outputted.
			 * @return Reference to the output stream.
			 */
			friend std::wostream& operator<<(std::wostream& p_out, const PushConstantsLayout& p_layout);
		};

		/**
		 * @brief Class to manage the layout of uniform blocks.
		 * 
		 * UniformBlockLayout is responsible for managing the layout of uniform blocks in shaders.
		 * It extends FieldArrayLayout and provides methods to treat shader instructions specific
		 * to uniform blocks.
		 */
		class UniformBlockLayout : public FieldArrayLayout
		{
		public:
			/**
			 * @brief Struct to serve as a key for identifying uniform blocks.
			 * 
			 * Key contains information about the set and binding of a uniform block, which are used to 
			 * uniquely identify a uniform block within a shader.
			 */
			struct Key
			{
				size_t binding;  ///< The binding point for the uniform block.
				size_t set;      ///< The descriptor set to which the uniform block belongs.

				/**
				 * @brief Constructor to initialize a Key object.
				 * 
				 * Initializes a Key object with specified set and binding values.
				 *
				 * @param p_set The descriptor set to which the uniform block belongs.
				 * @param p_binding The binding point for the uniform block.
				 */
				Key(size_t p_set = 0, size_t p_binding = 0);

				/**
				 * @brief Compares this Key object with another for sorting.
				 * 
				 * Compares the set and binding of this Key object with those of another Key object.
				 * Used for sorting Key objects.
				 *
				 * @param p_other The other Key object to compare with.
				 * @return True if this Key is considered less than the other, false otherwise.
				 */
				bool operator<(const Key &p_other) const;

				/**
				 * @brief Compares this Key object with another for equality.
				 * 
				 * Compares the set and binding of this Key object with those of another Key object.
				 * 
				 * @param p_other The other Key object to compare with.
				 * @return True if the Keys are equal, false otherwise.
				 */
				bool operator==(const Key &p_other) const;

				/**
				 * @brief Compares this Key object with another for inequality.
				 * 
				 * Compares the set and binding of this Key object with those of another Key object.
				 * 
				 * @param p_other The other Key object to compare with.
				 * @return True if the Keys are not equal, false otherwise.
				 */
				bool operator!=(const Key &p_other) const;
			};

			/**
			 * @brief Enumeration to define the mode of the uniform.
			 * 
			 * Specifies whether the uniform is part of a block or a single uniform.
			 */
			enum class Mode
			{
				Block,
				Single
			};

		private:
			std::wstring _name;  ///< The name of the uniform block.
			std::wstring _type;  ///< The type of the uniform block.
			Mode _mode;          ///< The mode of the uniform (Block or Single).
			Key _key;            ///< The key identifying the uniform block.

			/**
			 * @brief Internal function to treat a single uniform instruction.
			 * 
			 * @param p_instruction The shader instruction to be treated.
			 */
			void _treatSamplerUniform(const ShaderModule::Instruction &p_instruction);

			/**
			 * @brief Internal function to treat a uniform block instruction.
			 * 
			 * @param p_instruction The shader instruction to be treated.
			 */
			void _treatUniformBlock(const ShaderModule::Instruction &p_instruction);

		public:
			/**
			 * @brief Constructor for UniformBlockLayout.
			 * 
			 * Initializes the UniformBlockLayout with the associated StructureLayout.
			 *
			 * @param p_structureLayout Reference to the associated StructureLayout.
			 */
			UniformBlockLayout(const StructureLayout& p_structureLayout);

			/**
			 * @brief Treats a shader instruction and updates the uniform block layout.
			 * 
			 * Overridden function to treat a shader instruction specific to uniform blocks
			 * and updates the layout accordingly.
			 *
			 * @param p_instruction The shader instruction to be treated.
			 */
			void treat(const ShaderModule::Instruction &p_instruction);

			/**
			 * @brief Retrieves the name of the uniform block.
			 * 
			 * @return The name of the uniform block as a wide string.
			 */
			const std::wstring& name() const;

			/**
			 * @brief Retrieves the type of the uniform block.
			 * 
			 * @return The type of the uniform block as a wide string.
			 */
			const std::wstring& type() const;

			/**
			 * @brief Retrieves the key identifying the uniform block.
			 * 
			 * @return The key of the uniform block.
			 */
			const Key& key() const;

			/**
			 * @brief Retrieves the mode of the uniform (Block or Single).
			 * 
			 * @return The mode of the uniform.
			 */
			const Mode& mode() const;

			/**
			 * @brief Operator overload for formatted output of UniformBlockLayout::Mode.
			 *
			 * @param p_out The wide output stream where the formatted text will be written to.
			 * @param p_mode The UniformBlockLayout::Mode object to be formatted and outputted.
			 * @return Reference to the output stream.
			 */
			friend std::wostream& operator<<(std::wostream& p_out, const UniformBlockLayout::Mode& p_mode);

			/**
			 * @brief Operator overload for formatted output of UniformBlockLayout::Key.
			 *
			 * @param p_out The wide output stream where the formatted text will be written to.
			 * @param p_key The UniformBlockLayout::Key object to be formatted and outputted.
			 * @return Reference to the output stream.
			 */
			friend std::wostream& operator<<(std::wostream& p_out, const UniformBlockLayout::Key& p_key);

			/**
			 * @brief Operator overload for formatted output of UniformBlockLayout.
			 *
			 * @param p_out The wide output stream where the formatted text will be written to.
			 * @param p_block The UniformBlockLayout object to be formatted and outputted.
			 * @return Reference to the output stream.
			 */
			friend std::wostream& operator<<(std::wostream& p_out, const UniformBlockLayout& p_block);
		};

	private:
		/**
		 * @brief Mask for the types of instructions relevant to the vertex shader.
		 * 
		 * This mask is used to filter which types of instructions are processed for the vertex shader.
		 */
		int _vertexTypeMask = static_cast<int>(ShaderModule::Instruction::Type::Version) |
							   static_cast<int>(ShaderModule::Instruction::Type::Structure) |
							   static_cast<int>(ShaderModule::Instruction::Type::PushConstant) |
							   static_cast<int>(ShaderModule::Instruction::Type::SamplerUniform) |
							   static_cast<int>(ShaderModule::Instruction::Type::StorageBuffer) |
							   static_cast<int>(ShaderModule::Instruction::Type::UniformBlock);

		/**
		 * @brief Mask for the types of instructions relevant to the fragment shader.
		 * 
		 * This mask is used to filter which types of instructions are processed for the fragment shader.
		 */
		int _fragmentTypeMask = static_cast<int>(ShaderModule::Instruction::Type::Version) |
								static_cast<int>(ShaderModule::Instruction::Type::Structure) |
								static_cast<int>(ShaderModule::Instruction::Type::PushConstant) |
								static_cast<int>(ShaderModule::Instruction::Type::SamplerUniform) |
								static_cast<int>(ShaderModule::Instruction::Type::OutputBuffer) |
								static_cast<int>(ShaderModule::Instruction::Type::UniformBlock);

		ShaderModule _vertexModule; ///< Shader module for the vertex shader.
		ShaderModule _fragmentModule; ///< Shader module for the fragment shader.

		StructureLayout _structureLayout; ///< Layout for structures.
		StorageBufferLayout _storageBufferLayout; ///< Layout for storage buffers.
		OutputBufferLayout _outputBufferLayout; ///< Layout for output buffers.
		PushConstantsLayout _pushConstantsLayout; ///< Layout for push constants.
		std::vector<UniformBlockLayout> _uniformBlocksLayout; ///< Vector of layouts for uniform blocks.

		/**
		 * @brief Parses the version instruction from the shader.
		 * 
		 * Extracts and processes information related to the version of the shader.
		 *
		 * @param p_instruction The instruction to parse.
		 */
		void _parseVersion(const ShaderModule::Instruction& p_instruction);

		/**
		 * @brief Parses the storage buffer instruction from the shader.
		 * 
		 * Extracts and processes information related to storage buffers used in the shader.
		 *
		 * @param p_instruction The instruction to parse.
		 */
		void _parseStorageBuffer(const ShaderModule::Instruction& p_instruction);

		/**
		 * @brief Parses the output buffer instruction from the shader.
		 * 
		 * Extracts and processes information related to output buffers used in the shader.
		 *
		 * @param p_instruction The instruction to parse.
		 */
		void _parseOutputBuffer(const ShaderModule::Instruction& p_instruction);

		/**
		 * @brief Parses the push constant instruction from the shader.
		 * 
		 * Extracts and processes information related to push constants used in the shader.
		 *
		 * @param p_instruction The instruction to parse.
		 */
		void _parsePushConstant(const ShaderModule::Instruction& p_instruction);

		/**
		 * @brief Parses the uniform block instruction from the shader.
		 * 
		 * Extracts and processes information related to uniform blocks used in the shader.
		 *
		 * @param p_instruction The instruction to parse.
		 */
		void _parseUniformBlock(const ShaderModule::Instruction& p_instruction);

		/**
		 * @brief Parses the structure instruction from the shader.
		 * 
		 * Extracts and processes information related to structures used in the shader.
		 *
		 * @param p_instruction The instruction to parse.
		 */
		void _parseStructure(const ShaderModule::Instruction& p_instruction);

		/**
		 * @brief Parses the function instruction from the shader.
		 * 
		 * Extracts and processes information related to functions defined in the shader.
		 *
		 * @param p_instruction The instruction to parse.
		 */
		void _parseFunction(const ShaderModule::Instruction& p_instruction);

		/**
		 * @brief Parses the error instruction from the shader.
		 * 
		 * Extracts and processes error information from the shader instruction. This could be used for debugging or validation.
		 *
		 * @param p_instruction The instruction to parse.
		 */
		void _parseError(const ShaderModule::Instruction& p_instruction);

		/**
		 * @brief Parses a single shader instruction.
		 * 
		 * Delegates the instruction to specific parsers based on its type.
		 *
		 * @param p_instruction The shader instruction to parse.
		 */
		void _parseInstruction(const ShaderModule::Instruction& p_instruction);

		/**
		 * @brief Parses a set of shader instructions.
		 * 
		 * Iterates through the instruction set and delegates each one to specific parsers based on its type and a mask.
		 *
		 * @param p_instructionSet The set of shader instructions to parse.
		 * @param p_typeMask The mask specifying which types of instructions should be parsed.
		 */
		void _parseInstructionSet(const ShaderModule::InstructionSet &p_instructionSet, const int &p_typeMask);

	public:
		/**
		 * @brief Constructs a ShaderLayout object.
		 * 
		 * Initializes the ShaderLayout with the provided vertex and fragment shader modules.
		 *
		 * @param p_vertexInput The vertex shader module.
		 * @param p_fragmentInput The fragment shader module.
		 */
		ShaderLayout(const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput);

		/**
		 * @brief Retrieves the vertex shader module.
		 * @return A constant reference to the vertex shader module.
		 */
		const ShaderModule &vertexModule() const;

		/**
		 * @brief Retrieves the fragment shader module.
		 * @return A constant reference to the fragment shader module.
		 */
		const ShaderModule &fragmentModule() const;

		/**
		 * @brief Retrieves the structure layout.
		 * @return A constant reference to the structure layout.
		 */
		const StructureLayout &structureLayout() const;

		/**
		 * @brief Retrieves the storage buffer layout.
		 * @return A constant reference to the storage buffer layout.
		 */
		const StorageBufferLayout& storageBufferLayout() const;

		/**
		 * @brief Retrieves the output buffer layout.
		 * @return A constant reference to the output buffer layout.
		 */
		const OutputBufferLayout& outputBufferLayout() const;

		/**
		 * @brief Retrieves the push constants layout.
		 * @return A constant reference to the push constants layout.
		 */
		const PushConstantsLayout& pushConstantsLayout() const;

		/**
		 * @brief Retrieves the uniform block layouts.
		 * @return A constant reference to the vector of uniform block layouts.
		 */
		const std::vector<UniformBlockLayout>& uniformBlockLayouts() const;

		/**
		 * @brief Overloaded stream insertion operator for ShaderLayout.
		 * 
		 * Allows for easy printing of ShaderLayout objects.
		 *
		 * @param p_out The wide output stream.
		 * @param p_config The ShaderLayout object to insert into the stream.
		 * @return A reference to the modified wide output stream.
		 */
		friend std::wostream& operator<<(std::wostream& p_out, const ShaderLayout& p_config);

	};
}