#pragma once

#include <map>
#include <string>
#include <filesystem>
#include <unordered_map>

#include "data_structure/spk_data_buffer.hpp"
#include "spk_basic_functions.hpp"

namespace spk::GraphicalAPI
{
	class AbstractPipeline
	{
		friend class Object;

	public:
		class Configuration
		{
		public:
			struct Data
			{
				enum class Type
				{
					Error,
					Int,
					UInt,
					Float,
					Complex
				};
				Type type;
				size_t format;
				size_t size;

				Data();
				Data(const size_t &p_format, const Type &p_type);
				Data(const size_t &p_size);
				friend std::wostream &operator<<(std::wostream &os, const Type &p_type);
				friend std::wostream &operator<<(std::wostream &os, const Data &p_data);
			};

			struct StorageLayout
			{
				struct Field
				{
					size_t location;
					size_t offset;
					Data attribute;

					Field();
					Field(const Data &p_dataType, const size_t &p_location, const size_t &p_offset);
					friend std::wostream &operator<<(std::wostream &os, const Field &p_field);
				};

				size_t stride;
				std::vector<Field> fields;

				StorageLayout();
				StorageLayout(const std::vector<Field> &p_fields);
				friend std::wostream &operator<<(std::wostream &os, const StorageLayout &p_storageLayout);
			};

			struct PushConstantLayout
			{
				struct Field
				{
					size_t offset;
					std::wstring name;
					std::wstring type;
					Data attribute;

					Field();
					Field(const std::wstring& p_name, const Data &p_dataType, const size_t &p_offset);
					friend std::wostream &operator<<(std::wostream &os, const Field &p_field);
				};

				std::wstring name;
				std::wstring type;
				size_t stride;
				std::vector<Field> fields;

				PushConstantLayout();
				PushConstantLayout(const std::vector<Field> &p_fields);
				friend std::wostream &operator<<(std::wostream &os, const PushConstantLayout &p_pushConstantLayout);
			};

			struct UniformBlockLayout
			{
				struct Field
				{
					size_t offset;
					std::wstring name;
					Data attribute;

					Field();
					Field(const Data &p_dataType, const size_t &p_offset);
					friend std::wostream &operator<<(std::wostream &os, const Field &p_field);
				};

				std::wstring name;
				std::wstring type;
				size_t set;
				size_t binding;
				size_t stride;
				std::vector<Field> fields;

				UniformBlockLayout();
				UniformBlockLayout(const std::vector<Field> &p_fields);
				friend std::wostream &operator<<(std::wostream &os, const UniformBlockLayout &p_block);
			};

			std::map<std::string, Data> dataTypes;

			StorageLayout storage;
			PushConstantLayout constants;
			std::vector<UniformBlockLayout> uniformBlocks;

			bool isPushConstantUniformInstruction(const std::string &p_instruction);
			bool isUniformLayoutInstruction(const std::string &p_instruction);
			bool isLayoutBufferInInstruction(const std::string &p_instruction);
			
			void parseLayoutUniformInstruction_layoutInfo(UniformBlockLayout& p_uniformBlockToFill, const std::string &p_instruction);
			void parseLayoutUniformInstruction_blockType(UniformBlockLayout& p_uniformBlockToFill, const std::string &p_instruction);
			void parseLayoutUniformInstruction_content(UniformBlockLayout& p_uniformBlockToFill, const std::string &p_instruction);
			void parseLayoutUniformInstruction_instanceName(UniformBlockLayout& p_uniformBlockToFill, const std::string &p_instruction);
			void parseLayoutUniformInstruction(const std::string &p_instruction);

			void parseLayoutPushConstantInstruction_blockType(const std::string &p_instruction);
			void parseLayoutPushConstantInstruction_content(const std::string &p_instruction);
			void parseLayoutPushConstantInstruction_instanceName(const std::string &p_instruction);
			void parseLayoutPushConstantInstruction(const std::string &p_instruction);
			
			void parseLayoutBufferInstruction(const std::string &p_instruction);
			
			void parseLayoutInstruction(const std::string &p_instruction);

			void parseStructInstruction(const std::string &p_instruction);
			
			void parseShaderInstruction(const std::string &p_instruction);
			void parseShaderCode(const std::string &p_vertexCode);
			Configuration();
			Configuration(const std::string &p_vertexCode, const std::string &p_fragmentCode);
		};

		class Object
		{
			friend class std::shared_ptr<Object>;
			friend class AbstractPipeline;

		public:
			struct Storage
			{
			private:
				template <typename T, typename... Rest>
				struct UnitImpl : public UnitImpl<Rest...>
				{
					UnitImpl(const T &p_attribute, const Rest &...p_rest)
						: UnitImpl<Rest...>(p_rest...), attribute(p_attribute)
					{
					}

					T attribute;
				};

				template <typename T>
				struct UnitImpl<T>
				{
					UnitImpl(const T &p_attribute) : attribute(p_attribute)
					{
					}

					T attribute;
				};

			public:
				template <typename... Types>
				struct Unit : public UnitImpl<Types...>
				{
				public:
					Unit(const Types &...p_args) : UnitImpl<Types...>(p_args...)
					{
					}
				};

			private:
				spk::DataBuffer _content;
				const AbstractPipeline::Configuration::StorageLayout &_configuration;

			public:
				Storage(const AbstractPipeline::Configuration::StorageLayout &p_storageConfiguration);

				void clear();

				const void *data() const;
				const size_t size() const;

				template <typename... Types>
				Storage &operator<<(const Unit<Types...> &p_unit)
				{
					if (sizeof(p_unit) != _configuration.stride)
						spk::throwException(L"Unexpected unit size [" + std::to_wstring(sizeof(p_unit)) + L"] vs expected [" + std::to_wstring(_configuration.stride) + L"]");
					_content.append(&p_unit, sizeof(p_unit));

					return (*this);
				}

				template <typename... Types>
				Storage &operator<<(const std::vector<Unit<Types...>> &p_unitVector)
				{
					if (sizeof(p_unitVector[0]) != _configuration.stride)
						spk::throwException(L"Unexpected unit size [" + std::to_wstring(sizeof(p_unitVector[0])) + L"] vs expected [" + std::to_wstring(_configuration.stride) + L"]");
					_content.append(p_unitVector.data(), sizeof(Unit<Types...>) * p_unitVector.size());

					return (*this);
				}
			}; //? struct Storage

			struct Indexes
			{
			public:
				using Index = unsigned int;

			private:
				std::vector<Index> _content;

			public:
				Indexes();

				void clear();

				Indexes &operator<<(const Index &p_index);
				Indexes &operator<<(const std::vector<Index> &p_indexes);

				void insert(const Index *p_indexes, size_t p_size);

				const void *data() const;
				const size_t nbIndexes() const;
				const size_t size() const;
			};

			class PushConstants
			{
			public:
				class Field
				{
					friend class PushConstants;

				private:
					void *data;
					size_t size;

					Field(void *p_data, const size_t& p_size) :
						data(p_data),
						size(p_size)
					{

					}
				public:
					Field() :
						data(nullptr),
						size(0)
					{

					}

					template <typename TType>
					Field& operator << (const TType& p_newValue)
					{
						if (sizeof(TType) != size)
							spk::throwException(L"Field expected a size of [" + std::to_wstring(size) + L"] but user provided a data of size [" + std::to_wstring(sizeof(TType)) + L"]");
						std::memcpy(data, &p_newValue, size);
						return (*this);
					}
				};
			
			private:
				spk::DataBuffer _data;
				std::map<std::wstring, Field> _fields;

			public:
				PushConstants(const Configuration::PushConstantLayout& p_layout)
				{
					_data.resize(p_layout.stride);
					for (auto& field : p_layout.fields)
					{
						_fields[field.name] = Field(_data.data() + field.offset, field.attribute.size * field.attribute.format);
					}
				}

				Field& operator[](const std::wstring& p_name)
				{
					if (_fields.contains(p_name) == false)
						spk::throwException(L"PushConstants named [" + p_name + L"] doesn't exist");
					return (_fields[p_name]);
				}

				const void *data() const
				{
					return (_data.data());
				}
				const size_t size() const
				{
					return (_data.size());
				}

				template <typename TType>
				PushConstants& operator << (const TType& p_newValue)
				{
					if (sizeof(TType) != _data.size())
						spk::throwException(L"PushConstants expected a size of [" + std::to_wstring(_data.size()) + L"] but user provided a data of size [" + std::to_wstring(sizeof(TType)) + L"]");
					std::memcpy(_data.data(), static_cast<void*>(&p_newValue), _data.size());
					return (*this);
				}
			};

		private:
			AbstractPipeline *_owner;
			Storage _storage;
			Indexes _indexes;
			PushConstants _pushConstants;

		public:
			Object(AbstractPipeline *p_owner);

			virtual void push() = 0;
			virtual void updateConstants() = 0;

			virtual void activate() = 0;
			virtual void deactivate() = 0;

			void render();

			Storage &storage();
			Indexes &indexes();
			PushConstants &pushConstants();
		};

		class UniformBlock
		{
		public:
			class Field
			{
				friend class UniformBlock;

			private:
				std::wstring name;
				void *data;
				size_t size;

				Field(void *p_data, const size_t& p_size) :
					data(p_data),
					size(p_size)
				{

				}
			public:
				Field() :
					name(L""),
					data(nullptr),
					size(0)
				{

				}

				Field(const std::wstring& p_name) :
					name(p_name),
					data(nullptr),
					size(0)
				{

				}

				template <typename TType>
				Field& operator << (const TType& p_newValue)
				{
					if (sizeof(TType) != size)
						spk::throwException(L"UniformBlock::Field [" + name + L"] expected a size of [" + std::to_wstring(size) + L"] but user provided a data of size [" + std::to_wstring(sizeof(TType)) + L"]");
					std::memcpy(data, &p_newValue, size);
					return (*this);
				}
			};
		
		private:
			spk::DataBuffer _data;
			std::wstring _name;
			std::map<std::wstring, Field> _fields;

		public:
			UniformBlock(const Configuration::UniformBlockLayout& p_layout)
			{
				_name = p_layout.name;
				_data.resize(p_layout.stride);
				for (auto& field : p_layout.fields)
				{
					_fields[field.name] = Field(_data.data() + field.offset, field.attribute.size * field.attribute.format);
				}
			}

			Field& operator[](const std::wstring& p_name)
			{
				if (_fields.contains(p_name) == false)
					spk::throwException(L"Field [" + p_name + L"] inside UniformBlock [" + _name + L"] doesn't exist");
				return (_fields[p_name]);
			}

			const void *data() const
			{
				return (_data.data());
			}
			const size_t size() const
			{
				return (_data.size());
			}

			template <typename TType>
			UniformBlock& operator << (const TType& p_newValue)
			{
				this->push(p_newValue);
				return (*this);
			}

			template <typename TType>
			void push(const TType& p_newValue)
			{
				if (sizeof(TType) != _data.size())
					spk::throwException(L"UniformBlock [" + _name + L"] expected a size of [" + std::to_wstring(_data.size()) + L"] but user provided a data of size [" + std::to_wstring(sizeof(TType)) + L"]");
				std::memcpy(_data.data(), static_cast<const void*>(&p_newValue), _data.size());
			}

			virtual void update() = 0;
		};

	protected:
		Configuration _configuration;

		std::map<std::wstring, std::unique_ptr<UniformBlock>> _uniformBlocks;

		virtual void _loadProgram(
			const std::string &p_vertexName, const std::string &p_vertexCode,
			const std::string &p_fragmentName, const std::string &p_fragmentCode) = 0;

		void _loadAbstractPipeline(
			const std::string &p_vertexName, std::string& p_vertexCode,
			const std::string &p_fragmentName, std::string& p_fragmentCode);

		virtual std::unique_ptr<UniformBlock> _createUniformBlock(const Configuration::UniformBlockLayout& p_layout) = 0;

	public:
		AbstractPipeline();

		const Configuration &configuration() const;

		virtual void activate() = 0;
		virtual void deactivate() = 0;
		virtual void launch(const size_t &p_nbIndexes) = 0;

		void loadFromCode(std::string p_vertexCode, std::string p_fragmentCode);
		void loadFromFile(const std::filesystem::path &p_vertexShaderPath, const std::filesystem::path &p_fragmentShaderPath);

		std::unique_ptr<UniformBlock>& uniformBlock(const std::wstring& p_name);
		virtual std::unique_ptr<Object> createObject() = 0;
	};
}