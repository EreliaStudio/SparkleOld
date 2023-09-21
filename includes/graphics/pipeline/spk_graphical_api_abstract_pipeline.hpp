#pragma once

#include <map>
#include <string>
#include <filesystem>

#include "data_structure/spk_data_buffer.hpp"
#include "spk_basic_functions.hpp"

namespace spk::GraphicalAPI
{
	class AbstractPipeline
	{
	public:
		class Object
		{
			friend class AbstractPipeline;

		public:
			struct Storage
			{
			public:
				struct Configuration
				{
					struct Value
					{
						enum class Type
						{
							Float,
							Int,
							UInt
						};

						Type type = Type::Float;
						size_t location = 0;
						size_t offset = 0;
						size_t size = 0;
					};
					std::map<std::wstring, Value> values;
					size_t totalSize = 0;

					friend std::wostream& operator<<(std::wostream& p_out, const Configuration& p_config)
					{
						p_out << L"Total size: " << p_config.totalSize << std::endl;

						for (const auto& value : p_config.values) {
							p_out << L"Value [" << value.first << L"]:" << std::endl;
							p_out << L"    Location: " << value.second.location << std::endl;
							p_out << L"    Offset: " << value.second.offset << std::endl;
							p_out << L"    Type: " << [&]() -> std::wstring {
								switch (value.second.type) {
									case Value::Type::Float:
										return L"Float";
									case Value::Type::Int:
										return L"Int";
									case Value::Type::UInt:
										return L"UInt";
								}
								return L"Unknown"; // This should never happen
							}() << std::endl;
							p_out << L"    Size: " << value.second.size << std::endl;
						}
						return p_out;
					}
				}; //? struct Configuration

			private:
				template <typename T, typename... Rest>
				struct UnitImpl : public UnitImpl<Rest...>
				{
					UnitImpl(const T& p_value, const Rest &...p_rest)
						: UnitImpl<Rest...>(p_rest...), value(p_value)
					{
					}

					T value;
				};

				template <typename T>
				struct UnitImpl<T>
				{
					UnitImpl(const T& p_value) : value(p_value)
					{
					}

					T value;
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
				const Configuration& _configuration;

			public:
				Storage(const Storage::Configuration& p_storageConfiguration);

				template <typename... Types>
				Storage& operator<<(const Unit<Types...>& p_unit)
				{
					if (sizeof(p_unit) != _configuration.totalSize)
						spk::throwException(L"Unexpected unit size");
					_content.append(&p_unit, sizeof(p_unit));

					return (*this);
				}

				template <typename... Types>
				Storage& operator<<(const std::vector<Unit<Types...>>& p_unitVector)
				{
					if (sizeof(p_unitVector[0]) != _configuration.totalSize)
						spk::throwException(L"Unexpected unit size");
					_content.append(p_unitVector.data(), sizeof(Unit<Types...>) * p_unitVector.size());

					return (*this);
				}

				const void* data() const { return (_content.data()); }

				const size_t size() const { return (_content.size()); }
			}; //? struct Storage

		private:
			AbstractPipeline* _owner;
			Storage _storage;

			Object(AbstractPipeline* p_owner, const Storage::Configuration& p_storageConfiguration);

		public:
			Object(const Object& p_other) = delete;
			Object& operator=(const Object& p_other) = delete;

			Object(Object&& p_other) = default;
			Object& operator=(Object&& p_other) = default;

			void activate();
			void deactivate();
			void push();
			void render();
			Storage& storage();
		}; //? class Object

	protected:
		Object::Storage::Configuration _storageConfiguration;

		virtual void _loadProgram(
			const std::string& p_vertexName, const std::string& p_vertexCode,
			const std::string& p_fragmentName, const std::string& p_fragmentCode) = 0;

		virtual void _pushStorageData(const void* p_data, const size_t& p_dataSize) = 0;

		virtual void _renderObject(Object* p_object) = 0;

		virtual void _activateObject(Object* p_object) = 0;
		virtual void _deactivateObject(Object* p_object) = 0;

		Object::Storage::Configuration _parseStorageBuffers(const std::string& p_vertexModuleCode);

		void _loadAbstractPipeline(
			const std::string& p_vertexName, const std::string& p_vertexCode,
			const std::string& p_fragmentName, const std::string& p_fragmentCode);

	public:
		AbstractPipeline();

		void loadFromCode(const std::string& p_vertexCode, const std::string& p_fragmentCode);
		void loadFromFile(const std::filesystem::path& p_vertexShaderPath, const std::filesystem::path& p_fragmentShaderPath);

		Object createObject();
	}; //? class AbstractPipeline
}