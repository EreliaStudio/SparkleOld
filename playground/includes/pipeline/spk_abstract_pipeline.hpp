#pragma once

#include "sparkle.hpp"

#include "pipeline/spk_shader_module.hpp"

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
			std::map<std::string, Data> _singleUniformStructures;
			std::wstring _acceptedSingleUniformTypeString;

		public:
			StructureLayout();
			void reset();

			void treat(const ShaderModule::Instruction &p_instruction);

			const std::wstring& acceptedSingleUniformTypeString() const;
			const std::map<std::string, Data>& structures() const;
			const std::map<std::string, Data>& singleUniformStructures() const;
		};

		class FieldArrayLayout
		{
		public:
			struct Field
			{
				std::string name;
				Data data;
				size_t location;
				size_t offset;
			};
		private:
			size_t _stride;
			std::vector<Field> _fields;
			
		protected:
			const StructureLayout& structureLayout;

		public:

			FieldArrayLayout(const StructureLayout& p_structureLayout);

			void insert(const std::string& p_name, const Data& p_data, const size_t& p_location = 0);

			virtual void treat(const ShaderModule::Instruction &p_instruction) = 0;

			const size_t& stride() const;
			const std::vector<Field>& fields() const ;
		};

		class StorageBufferLayout : public FieldArrayLayout
		{
		public:
			StorageBufferLayout(const StructureLayout& p_structureLayout);

			void treat(const ShaderModule::Instruction &p_instruction);
		};

		class OutputBufferLayout : public FieldArrayLayout
		{
		public:
			OutputBufferLayout(const StructureLayout& p_structureLayout);

			void treat(const ShaderModule::Instruction &p_instruction);
		};

		class PushConstantLayout : public FieldArrayLayout
		{
		private:
			std::wstring _type;
			std::wstring _name;

		public:
			PushConstantLayout(const StructureLayout& p_structureLayout);

			void treat(const ShaderModule::Instruction &p_instruction);
		};

		class UniformBlock : public FieldArrayLayout
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
				Single
			};

		private:
			Mode _mode;
			Key _key;

			void _treatSingleUniform(const ShaderModule::Instruction &p_instruction);
			void _treatUniformBlock(const ShaderModule::Instruction &p_instruction);

		public:
			UniformBlock(const StructureLayout& p_structureLayout);

			void treat(const ShaderModule::Instruction &p_instruction);

			const Key& key() const;
			const Mode& mode() const;
		};

	private:
		int _vertexTypeMask = static_cast<int>(ShaderModule::Instruction::Type::Version) |
							  static_cast<int>(ShaderModule::Instruction::Type::Structure) |
							  static_cast<int>(ShaderModule::Instruction::Type::PushConstant) |
							  static_cast<int>(ShaderModule::Instruction::Type::SingleUniform) |
							  static_cast<int>(ShaderModule::Instruction::Type::StorageBuffer) |
							  static_cast<int>(ShaderModule::Instruction::Type::UniformBlock);

		int _fragmentTypeMask = static_cast<int>(ShaderModule::Instruction::Type::Version) |
								static_cast<int>(ShaderModule::Instruction::Type::Structure) |
								static_cast<int>(ShaderModule::Instruction::Type::PushConstant) |
								static_cast<int>(ShaderModule::Instruction::Type::SingleUniform) |
								static_cast<int>(ShaderModule::Instruction::Type::OutputBuffer) |
								static_cast<int>(ShaderModule::Instruction::Type::UniformBlock);

		StructureLayout _structureLayout;
		StorageBufferLayout _storageBufferLayout;
		OutputBufferLayout _outputBufferLayout;
		PushConstantLayout _pushConstantLayout;
		std::vector<UniformBlock> _uniformBlocks;

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

		const StorageBufferLayout& storageBufferLayout() const;
		const OutputBufferLayout& outputBufferLayout() const;
		const PushConstantLayout& pushConstantLayout() const;
		const std::vector<UniformBlock>& uniformBlocks() const;
	};

protected:
	virtual void _loadProgram(const Configuration &p_configuration, const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput) = 0;

	void _loadPipeline(const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput);

public:
};