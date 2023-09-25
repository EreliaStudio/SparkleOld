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
		friend class Object;

	public:
		class Object
		{
			friend class std::shared_ptr<Object>;
			friend class AbstractPipeline;

		public:
			struct Storage
			{
			public:
				struct Configuration
				{
					enum class Mode
					{
						Error,
						Data,
						Indexes,
						ShaderStorage,
						Texture
					};

					struct Attribute
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
						size_t format = 0;
						size_t unitSize = 0;
					};
					Mode mode = Mode::Error;
					size_t stride = 0;
					std::map<std::wstring, Attribute> attributes;

					friend std::wostream& operator<<(std::wostream& p_out, const Configuration& p_config);

					Configuration()
					{

					}

					Configuration(const Mode& p_mode)
					{
						mode = p_mode;
						stride = 0;
					}

					Configuration(const Mode& p_mode, const std::map<std::wstring, Attribute>& p_attributes)
					{
						static const std::map<AbstractPipeline::Object::Storage::Configuration::Attribute::Type, size_t> typeToSize = {
							{AbstractPipeline::Object::Storage::Configuration::Attribute::Type::Float, sizeof(float)},
							{AbstractPipeline::Object::Storage::Configuration::Attribute::Type::UInt, sizeof(unsigned int)},
							{AbstractPipeline::Object::Storage::Configuration::Attribute::Type::Int, sizeof(int)}
						}; 
						
						mode = p_mode;
						for (const auto& attribute : p_attributes)
						{
							stride += attribute.second.format * typeToSize.at(attribute.second.type);
						}
						attributes = p_attributes;
					}

					void inverseOffset()
					{
						for (auto& attribute : attributes)
						{
							attribute.second.offset = stride - attribute.second.offset - attribute.second.format * attribute.second.unitSize;
						}
					}
				}; //? struct Configuration

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
				const Configuration& _configuration;

			public:
				Storage(const Storage::Configuration& p_storageConfiguration);

				void clear();

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

				const void* data() const { return (_content.data()); }

				const size_t size() const { return (_content.size()); }
			}; //? struct Storage

			struct Indexes
			{
			public:
				using Index = unsigned int;

			private:
				std::vector<Index> _content;

			public:
				Indexes()
				{

				}

				void clear()
				{
					_content.clear();
				}

				Indexes &operator<<(const Index &p_index)
				{
					_content.push_back(p_index);
					return *this;
				}

				Indexes &operator<<(const std::vector<Index> &p_indexes)
				{
					_content.insert(_content.end(), p_indexes.begin(), p_indexes.end());
					return *this;
				}

				void insert(const Index* p_indexes, size_t p_size)
				{
					_content.insert(_content.end(), p_indexes, p_indexes + p_size);
				}

				const void* data() const { return (_content.data()); }
				const size_t nbIndexes() const { return (_content.size());}
				const size_t size() const { return (_content.size() * sizeof(unsigned int)); }
			};

		private:
			AbstractPipeline* _owner;
			Storage _storage;
			Indexes _indexes;

		public:
			Object(AbstractPipeline* p_owner, const Storage::Configuration& p_storageConfiguration);

			virtual void push() = 0;

			virtual void activate() = 0;
			virtual void deactivate() = 0;

			void render()
			{
				_owner->activate();
				activate();
				_owner->launch(_indexes.nbIndexes());
				deactivate();
				_owner->deactivate();
			}

			Storage& storage();
			Indexes& indexes();
		}; //? class Object

	protected:
		Object::Storage::Configuration _storageConfiguration;

		virtual void _loadProgram(
			const std::string& p_vertexName, const std::string& p_vertexCode,
			const std::string& p_fragmentName, const std::string& p_fragmentCode) = 0;

		Object::Storage::Configuration _parseStorageBuffers(const std::string& p_vertexModuleCode);

		void _loadAbstractPipeline(
			const std::string& p_vertexName, const std::string& p_vertexCode,
			const std::string& p_fragmentName, const std::string& p_fragmentCode);

	public:
		AbstractPipeline();

		virtual void activate() = 0;
		virtual void deactivate() = 0;
		virtual void launch(const size_t &p_nbIndexes) = 0;

		void loadFromCode(const std::string& p_vertexCode, const std::string& p_fragmentCode);
		void loadFromFile(const std::filesystem::path& p_vertexShaderPath, const std::filesystem::path& p_fragmentShaderPath);

		virtual std::shared_ptr<Object> createObject() = 0;
	};
}