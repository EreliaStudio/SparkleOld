#pragma once

#include "graphics/pipeline/spk_shader_module.hpp"
#include <map>

namespace spk
{
	class ShaderLayout
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

			friend std::wostream& operator<<(std::wostream& p_out, const Type& p_data);
			friend std::wostream& operator<<(std::wostream& p_out, const Data& p_data);
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

			friend std::wostream& operator<<(std::wostream& p_out, const StructureLayout& p_layout);
		};

		class FieldArrayLayout
		{
		public:
			struct Field
			{
				std::string name;
				Data data;
				int location = -1;
				size_t offset;

				friend std::wostream& operator<<(std::wostream& p_out, const Field& p_layout);
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

			friend std::wostream& operator<<(std::wostream& p_out, const FieldArrayLayout& p_layout);
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

		class PushConstantsLayout : public FieldArrayLayout
		{
		private:
			std::wstring _type;
			std::wstring _name;

		public:
			PushConstantsLayout(const StructureLayout& p_structureLayout);

			void treat(const ShaderModule::Instruction &p_instruction);

			friend std::wostream& operator<<(std::wostream& p_out, const PushConstantsLayout& p_layout);

			const std::wstring& type() const;
			const std::wstring& name() const;
		};

		class UniformBlockLayout : public FieldArrayLayout
		{
		public:
			struct Key
			{
				size_t binding;
				size_t set;

				Key(size_t p_set = 0, size_t p_binding = 0);

				bool operator<(const Key &p_other) const;
				bool operator==(const Key &p_other) const;
				bool operator!=(const Key &p_other) const;
			};

			enum class Mode
			{
				Block,
				Single
			};

		private:
			std::wstring _name;
			std::wstring _type;
			Mode _mode;
			Key _key;

			void _treatSingleUniform(const ShaderModule::Instruction &p_instruction);
			void _treatUniformBlock(const ShaderModule::Instruction &p_instruction);

		public:
			UniformBlockLayout(const StructureLayout& p_structureLayout);

			void treat(const ShaderModule::Instruction &p_instruction);

			const std::wstring& name() const;
			const std::wstring& type() const;
			const Key& key() const;
			const Mode& mode() const;

			friend std::wostream& operator<<(std::wostream& p_out, const UniformBlockLayout::Mode& p_mode);
			friend std::wostream& operator<<(std::wostream& p_out, const UniformBlockLayout::Key& p_key);
			friend std::wostream& operator<<(std::wostream& p_out, const UniformBlockLayout& p_block);
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

		ShaderModule _vertexModule;
		ShaderModule _fragmentModule;

		StructureLayout _structureLayout;
		StorageBufferLayout _storageBufferLayout;
		OutputBufferLayout _outputBufferLayout;
		PushConstantsLayout _pushConstantsLayout;
		std::vector<UniformBlockLayout> _uniformBlocksLayout;

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
		ShaderLayout(const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput);

		const ShaderModule &vertexModule() const;
		const ShaderModule &fragmentModule() const;

		const StructureLayout &structureLayout() const;
		const StorageBufferLayout& storageBufferLayout() const;
		const OutputBufferLayout& outputBufferLayout() const;
		const PushConstantsLayout& pushConstantsLayout() const;
		const std::vector<UniformBlockLayout>& uniformBlockLayouts() const;

		friend std::wostream& operator<<(std::wostream& p_out, const ShaderLayout& p_config);
	};
}