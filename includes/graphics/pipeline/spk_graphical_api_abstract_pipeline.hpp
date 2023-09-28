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
		struct Configuration
		{
			struct Data
			{
				size_t size = 0;

				Data();
				Data(const size_t& p_size);
			};

			struct StorageLayout
			{
				struct Field
				{
					size_t location = 0;
					size_t offset = 0;
					Data attribute;

					Field();
					Field(const Data& p_dataType, const size_t& p_location, const size_t& p_offset);
				};

				size_t stride;
				std::vector<Field> fields;

				StorageLayout();
				StorageLayout(const std::vector<Field> &p_fields);
			};

			struct PushConstantLayout
			{
				struct Field
				{
					size_t offset = 0;
					Data attribute;

					Field();
					Field(const Data& p_dataType, const size_t& p_offset);
				};

				size_t size;
				std::vector<Field> attributes;
			};

			struct UniformBlockLayout
			{
				struct Field
				{
					size_t offset = 0;
					Data attribute;

					Field();
					Field(const Data& p_dataType, const size_t& p_offset);
				};

				size_t size;
				std::vector<Field> attributes;
			};

			std::map<std::string, Data> dataTypes;

			StorageLayout storage;
			std::vector<UniformBlockLayout> uniforms;
			PushConstantLayout constants;

			Configuration();
			Configuration(const std::string& p_vertexCode, const std::string& p_fragmentCode);
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
					UnitImpl(const T& p_attribute, const Rest &...p_rest)
						: UnitImpl<Rest...>(p_rest...), attribute(p_attribute)
					{
					}

					T attribute;
				};

				template <typename T>
				struct UnitImpl<T>
				{
					UnitImpl(const T& p_attribute) : attribute(p_attribute)
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
				const AbstractPipeline::Configuration::StorageLayout& _configuration;

			public:
				Storage(const AbstractPipeline::Configuration::StorageLayout& p_storageConfiguration);

				void clear();

				const void* data() const;
				const size_t size() const;

				template <typename... Types>
				Storage& operator<<(const Unit<Types...>& p_unit)
				{
					if (sizeof(p_unit) != _configuration.stride)
						spk::throwException(L"Unexpected unit size [" + std::to_wstring(sizeof(p_unit)) + L"] vs expected [" + std::to_wstring(_configuration.stride) + L"]");
					_content.append(&p_unit, sizeof(p_unit));

					return (*this);
				}

				template <typename... Types>
				Storage& operator<<(const std::vector<Unit<Types...>>& p_unitVector)
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

				void insert(const Index* p_indexes, size_t p_size);

				const void* data() const;
				const size_t nbIndexes() const;
				const size_t size() const;
			};

		private:
			AbstractPipeline* _owner;
			Storage _storage;
			Indexes _indexes;

		public:
			Object(AbstractPipeline* p_owner);

			virtual void push() = 0;

			virtual void activate() = 0;
			virtual void deactivate() = 0;

			void render();

			Storage& storage();
			Indexes& indexes();
		};

	protected:
		Configuration _configuration;

		virtual void _loadProgram(
			const std::string& p_vertexName, const std::string& p_vertexCode,
			const std::string& p_fragmentName, const std::string& p_fragmentCode) = 0;

		void _loadAbstractPipeline(
			const std::string& p_vertexName, const std::string& p_vertexCode,
			const std::string& p_fragmentName, const std::string& p_fragmentCode);

	public:
		AbstractPipeline();

		const Configuration& configuration() const;

		virtual void activate() = 0;
		virtual void deactivate() = 0;
		virtual void launch(const size_t &p_nbIndexes) = 0;

		void loadFromCode(const std::string& p_vertexCode, const std::string& p_fragmentCode);
		void loadFromFile(const std::filesystem::path& p_vertexShaderPath, const std::filesystem::path& p_fragmentShaderPath);

		virtual std::shared_ptr<Object> createObject() = 0;
	};
}