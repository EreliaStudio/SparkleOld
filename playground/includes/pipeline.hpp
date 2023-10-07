#pragma once

#include "sparkle.hpp"

class AbstractPipeline
{
public:
	struct Input
	{
		std::string name;
		std::string code;

		void checkCodeValidity();
		Input(const std::filesystem::path &p_filePath);
		Input(const std::string &p_name, const std::string &p_code);
	};

	class Configuration
	{
	public:
		class InstructionSet
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

		private:
			std::vector<Instruction> _instructions;

			std::string compactify(const std::string &p_code);
			Instruction::Type determineInstructionType(const std::string &p_instruction);
			void parseInstructions(const std::string &p_code);

		public:
			InstructionSet(const Input &p_input);
			const std::vector<Instruction>& instructions() const;
		};
		
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

			void treat(const InstructionSet::Instruction &p_instruction);

			const std::map<std::string, Data>& structures() const;
			const std::map<std::string, Data>& standaloneStructures() const;
		};

		class ConfigurationLayout
		{
		protected:
			const std::map<std::string, Data>& structures;
			const std::map<std::string, Data>& standaloneStructures;

		public:
			ConfigurationLayout(const std::map<std::string, Data>& p_structures, const std::map<std::string, Data>& p_standaloneStructures);

			virtual void treat(const InstructionSet::Instruction &p_instruction) = 0;
		};

		class StorageBufferLayout : public ConfigurationLayout
		{
		public:
			struct Field
			{
				Data data;
				size_t location;
				size_t offset;

				Field(const Data& p_data = Data(), const size_t& p_location = 0, const size_t& p_offset = 0);
			};

		private:
			size_t _stride;
			std::vector<Field> _fields;

		public:
			StorageBufferLayout(const std::map<std::string, Data>& p_structures, const std::map<std::string, Data>& p_standaloneStructures);

			void treat(const InstructionSet::Instruction &p_instruction);

			const size_t& stride() const;
			const std::vector<Field>& fields() const ;
		};

		class PushConstantLayout : public ConfigurationLayout
		{
		public:
			struct Field
			{
				Data data;
				size_t offset;

				Field(const Data& p_data = Data(), const size_t& p_offset = 0);
			};

		private:
			size_t _stride;
			std::vector<Field> _fields;

		public:
			PushConstantLayout(const std::map<std::string, Data>& p_structures, const std::map<std::string, Data>& p_standaloneStructures);

			void treat(const InstructionSet::Instruction &p_instruction);

			const size_t& stride() const;
			const std::vector<Field>& fields() const ;
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

				void treat(const InstructionSet::Instruction &p_instruction);

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

			void treat(const InstructionSet::Instruction &p_instruction);

			const std::vector<UniformBlock::Key>& subscribedUniformBlocks() const;
		};

		class OutputBufferLayout : public ConfigurationLayout
		{
		public:
			struct Field
			{
				Data data;
				size_t location;
				size_t offset;

				Field(const Data& p_data = Data(), const size_t& p_location = 0, const size_t& p_offset = 0);
			};

		private:
			size_t _stride;
			std::vector<Field> _fields;

		public:
			OutputBufferLayout(const std::map<std::string, Data>& p_structures, const std::map<std::string, Data>& p_standaloneStructures);

			void treat(const InstructionSet::Instruction &p_instruction);

			const size_t& stride() const;
			const std::vector<Field>& fields() const;
		};

	private:
		int _vertexTypeMask = static_cast<int>(InstructionSet::Instruction::Type::Version) |
							  static_cast<int>(InstructionSet::Instruction::Type::Structure) |
							  static_cast<int>(InstructionSet::Instruction::Type::PushConstant) |
							  static_cast<int>(InstructionSet::Instruction::Type::SamplerUniform) |
							  static_cast<int>(InstructionSet::Instruction::Type::StorageBuffer) |
							  static_cast<int>(InstructionSet::Instruction::Type::UniformBlock);

		int _fragmentTypeMask = static_cast<int>(InstructionSet::Instruction::Type::Version) |
								static_cast<int>(InstructionSet::Instruction::Type::Structure) |
								static_cast<int>(InstructionSet::Instruction::Type::PushConstant) |
								static_cast<int>(InstructionSet::Instruction::Type::SamplerUniform) |
								static_cast<int>(InstructionSet::Instruction::Type::OutputBuffer) |
								static_cast<int>(InstructionSet::Instruction::Type::UniformBlock);

		StructureLayout _structureLayout;
		StorageBufferLayout _storageBufferLayout;
		OutputBufferLayout _outputBufferLayout;
		PushConstantLayout _pushConstantLayout;
		UniformBlockLayout _uniformBlockLayout;

		void _parseVersion(const InstructionSet::Instruction& p_instruction);
		void _parseStorageBuffer(const InstructionSet::Instruction& p_instruction);
		void _parseOutputBuffer(const InstructionSet::Instruction& p_instruction);
		void _parsePushConstant(const InstructionSet::Instruction& p_instruction);
		void _parseUniformBlock(const InstructionSet::Instruction& p_instruction);
		void _parseStructure(const InstructionSet::Instruction& p_instruction);
		void _parseFunction(const InstructionSet::Instruction& p_instruction);
		void _parseError(const InstructionSet::Instruction& p_instruction);
		
		void _parseInstruction(const InstructionSet::Instruction& p_instruction);
		void _parseInstructionSet(const InstructionSet &p_instructionSet, const int &p_typeMask);

	public:
		Configuration(const Input &p_vertexInput, const Input &p_fragmentInput);
	};

protected:
	virtual void _loadProgram(const Configuration &p_configuration, const Input &p_vertexInput, const Input &p_fragmentInput) = 0;

	void _loadPipeline(const Input &p_vertexInput, const Input &p_fragmentInput);

public:
};