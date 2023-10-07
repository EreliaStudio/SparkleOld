#pragma once

#include "sparkle.hpp"

class ShaderModule
{
public:
	class Instruction
	{
	public:
		enum class Type
		{
			Version = 0b0000000000000001,
			StorageBuffer = 0b0000000000000010,
			OutputBuffer = 0b0000000000000100,
			PushConstant = 0b0000000000001000,
			SamplerUniform = 0b0000000000010000,
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

class AbstractPipeline
{
public:
	class Configuration
	{
	public:
		struct Data
		{
			enum class Type
			{
				Int,
				UInt,
				Float,
				Structure,
				Error
			};

			Type type;
			size_t format;
			size_t size;

			Data(const Type& p_type = Type::Error, const size_t& p_format = 0, const size_t& p_size = 0);
			Data(const size_t& p_size);
		};

		class StructureLayout
		{
		private:
			std::map<std::string, Data> _structures;
			std::map<std::string, Data> _standaloneStructures;

		public:
			StructureLayout();
			void reset();

			void treat(const ShaderModule::Instruction &p_instruction);

			const std::map<std::string, Data>& structures() const;
			const std::map<std::string, Data>& standaloneStructures() const;
		};

		class ConfigurationLayout
		{
		public:
			struct Field
			{
				Data data;
				size_t location;
				size_t offset;

				Field(const Data& p_data, const size_t& p_offset);
				Field(const Data& p_data, const size_t& p_location, const size_t& p_offset);
			};
		protected:
			const std::map<std::string, Data>& structures;
			const std::map<std::string, Data>& standaloneStructures;

		public:
			ConfigurationLayout(const std::map<std::string, Data>& p_structures, const std::map<std::string, Data>& p_standaloneStructures);

			virtual void treat(const ShaderModule::Instruction &p_instruction) = 0;
		};

		class FieldArrayLayout : public ConfigurationLayout
		{
		protected:
			size_t _stride;
			std::vector<Field> _fields;

		public:
			FieldArrayLayout(const std::map<std::string, Data>& p_structures, const std::map<std::string, Data>& p_standaloneStructures);

			const size_t& stride() const;
			const std::vector<Field>& fields() const ;
		};

		class StorageBufferLayout : public FieldArrayLayout
		{
		public:
			StorageBufferLayout(const std::map<std::string, Data>& p_structures, const std::map<std::string, Data>& p_standaloneStructures);

			void treat(const ShaderModule::Instruction &p_instruction);
		};

		class OutputBufferLayout : public FieldArrayLayout
		{
		public:
			OutputBufferLayout(const std::map<std::string, Data>& p_structures, const std::map<std::string, Data>& p_standaloneStructures);

			void treat(const ShaderModule::Instruction &p_instruction);
		};

		class PushConstantLayout : public FieldArrayLayout
		{
		public:
			PushConstantLayout(const std::map<std::string, Data>& p_structures, const std::map<std::string, Data>& p_standaloneStructures);

			void treat(const ShaderModule::Instruction &p_instruction);
		};

		class UniformBlockLayout : public ConfigurationLayout
		{	
		public:
			class UniformBlock
			{
			public:
				struct Key
				{
					size_t binding;
					size_t set;

					Key(size_t p_set = 0, size_t p_binding = 0);

					bool operator<(const Key &p_other) const;
				};

				enum class Mode
				{
					Block,
					Sampler
				};

				struct Field
				{
					Data data;
					size_t offset;

					Field(const Data& p_data = Data(), const size_t& p_offset = 0);
				};

			private:
				Mode _mode;
				Key _key;
				size_t _stride;
				std::vector<Field> _fields;

			public:
				UniformBlock();

				void treat(const ShaderModule::Instruction &p_instruction);

				const Key& key() const;
				const Mode& mode() const;
				const size_t& stride() const;
				const std::vector<Field>& fields() const;
			};

			static inline std::map<UniformBlock::Key, UniformBlock> uniformBlocks;
			static inline std::map<std::wstring, UniformBlock::Key> uniformBlockKeys;

		private:
			std::vector<UniformBlock::Key> _subscribedUniformBlocks;

		public:
			UniformBlockLayout(const std::map<std::string, Data>& p_structures, const std::map<std::string, Data>& p_standaloneStructures);

			void treat(const ShaderModule::Instruction &p_instruction);

			const std::vector<UniformBlock::Key>& subscribedUniformBlocks() const;
		};

	private:
		int _vertexTypeMask = static_cast<int>(ShaderModule::Instruction::Type::Version) |
							  static_cast<int>(ShaderModule::Instruction::Type::Structure) |
							  static_cast<int>(ShaderModule::Instruction::Type::PushConstant) |
							  static_cast<int>(ShaderModule::Instruction::Type::SamplerUniform) |
							  static_cast<int>(ShaderModule::Instruction::Type::StorageBuffer) |
							  static_cast<int>(ShaderModule::Instruction::Type::UniformBlock);

		int _fragmentTypeMask = static_cast<int>(ShaderModule::Instruction::Type::Version) |
								static_cast<int>(ShaderModule::Instruction::Type::Structure) |
								static_cast<int>(ShaderModule::Instruction::Type::PushConstant) |
								static_cast<int>(ShaderModule::Instruction::Type::SamplerUniform) |
								static_cast<int>(ShaderModule::Instruction::Type::OutputBuffer) |
								static_cast<int>(ShaderModule::Instruction::Type::UniformBlock);

		StructureLayout _structureLayout;
		StorageBufferLayout _storageBufferLayout;
		OutputBufferLayout _outputBufferLayout;
		PushConstantLayout _pushConstantLayout;
		UniformBlockLayout _uniformBlockLayout;

		void _parseVersion(const ShaderModule::Instruction& p_instruction);
		void _parseStorageBuffer(const ShaderModule::Instruction& p_instruction);
		void _parseOutputBuffer(const ShaderModule::Instruction& p_instruction);
		void _parsePushConstant(const ShaderModule::Instruction& p_instruction);
		void _parseUniformBlock(const ShaderModule::Instruction& p_instruction);
		void _parseStructure(const ShaderModule::Instruction& p_instruction);
		void _parseFunction(const ShaderModule::Instruction& p_instruction);
		void _parseError(const ShaderModule::Instruction& p_instruction);
		
		void _parseInstruction(const ShaderModule::Instruction& p_instruction);
		void _parseInstructionSet(const ShaderModule::InstructionSet &p_instructionSet, const int &p_typeMask);

	public:
		Configuration(const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput);
	};

protected:
	virtual void _loadProgram(const Configuration &p_configuration, const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput) = 0;

	void _loadPipeline(const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput);

public:
};