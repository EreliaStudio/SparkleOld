#pragma once

#include "data_structure/spk_data_buffer.hpp"
#include "data_structure/spk_field_map.hpp"
#include "graphics/pipeline/spk_shader_module.hpp"
#include "graphics/pipeline/spk_shader_layout.hpp"
#include <map>

#include "spk_basic_functions.hpp"

namespace spk
{
	class AbstractPipeline
	{
	public:
		class Object
		{
		public:
			class Storage
			{
			public:
				class Buffer
				{
				private:
					std::wstring _name;
					size_t _unitSize;
					spk::DataBuffer _data;

				public:
					Buffer(const std::wstring &p_name, size_t p_unitSize) : _unitSize(p_unitSize),
																			_name(p_name)
					{
					}

					template <typename TType>
					Buffer &operator<<(const TType &p_data)
					{
						if (_unitSize != sizeof(TType))
						{
							spk::throwException(L"Pushing a unit of size [" + std::to_wstring(sizeof(TType)) + L"] into a " + _name + L" buffer of unit size [" + std::to_wstring(_unitSize) + L"]");
						}
						_data << p_data;
						return (*this);
					}

					template <typename TType>
					Buffer &operator<<(const std::vector<TType> &p_data)
					{
						if (_unitSize != sizeof(TType))
						{
							spk::throwException(L"Pushing a unit of size [" + std::to_wstring(sizeof(TType)) + L"] into a " + _name + L" buffer of unit size [" + std::to_wstring(_unitSize) + L"]");
						}
						_data.append(p_data.data(), p_data.size() * sizeof(TType));
						return (*this);
					}

					const uint8_t *data() const
					{
						return (_data.data());
					}

					size_t size() const
					{
						return (_data.size());
					}
				};

			private:
				Buffer _vertices;
				Buffer _indexes;

			public:
				Storage(const spk::ShaderLayout::StorageBufferLayout& p_storageBufferLayout) :
					_vertices(L"Vertices", p_storageBufferLayout.stride()),
					_indexes(L"Indexes", sizeof(size_t))
				{
				}

				Buffer &vertices()
				{
					return (_vertices);
				}

				Buffer &indexes()
				{
					return (_indexes);
				}
			};

			class PushConstants : public spk::FieldMap
			{
			public:
				PushConstants(const spk::ShaderLayout::PushConstantsLayout& p_pushConstantsLayout) :
					FieldMap(p_pushConstantsLayout.stride())
				{
					for (const auto& field : p_pushConstantsLayout.fields())
					{
						insertNewField(spk::to_wstring(field.name), field.offset, field.data.size * field.data.format);
					}
				}
			};

		private:
			AbstractPipeline* _owner;
			Storage _storage;
			size_t _nbIndexesPushed;
			PushConstants _pushConstants;

			virtual void _pushVerticesData(const uint8_t* p_data, size_t p_dataSize) = 0;
			virtual void _pushIndexesData(const uint8_t* p_data, size_t p_dataSize) = 0;
			virtual void _pushPushConstantsData(const uint8_t* p_data, size_t p_dataSize) = 0;
			virtual void _onRender() = 0;

		public:
			Object(AbstractPipeline* p_owner, const spk::ShaderLayout::StorageBufferLayout& p_storageBufferLayout, const spk::ShaderLayout::PushConstantsLayout& p_pushConstantsLayout) :
				_owner(p_owner),
				_storage(p_storageBufferLayout),
				_pushConstants(p_pushConstantsLayout),
				_nbIndexesPushed(0)
			{

			}

			void updateVertices()
			{
				_pushVerticesData(_storage.vertices().data(), _storage.vertices().size());
			}

			void updateIndexes()
			{
				_pushIndexesData(_storage.indexes().data(), _storage.indexes().size());
				_nbIndexesPushed = _storage.indexes().size() / sizeof(size_t);
			}

			void updatePushConstants()
			{
				_pushPushConstantsData(_pushConstants.data(), _pushConstants.size());
			}

			void render()
			{
				_owner->activate();

				_onRender();

				_owner->launch(_nbIndexesPushed);

				_owner->deactivate();
			}

			Storage& storage()
			{
				return (_storage);
			}

			PushConstants& pushConstants()
			{
				return (_pushConstants);
			}
		};

	protected:
		ShaderLayout _shaderLayout;

		virtual void _loadProgram(const ShaderLayout &p_shaderLayout) = 0;
		void _loadPipeline()
		{
			_loadProgram(_shaderLayout);
		}

		virtual std::shared_ptr<Object> _loadObject(const ShaderLayout::StorageBufferLayout& p_storageLayout, const ShaderLayout::PushConstantsLayout& p_pushConstantsLayout) = 0;

	public:
		AbstractPipeline(const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput) : _shaderLayout(p_vertexInput, p_fragmentInput)
		{

		}

		virtual void launch(const size_t &p_nbIndexes) = 0;

		virtual void activate() = 0;
		virtual void deactivate() = 0;

		std::shared_ptr<Object> createObject()
		{
			return (_loadObject(_shaderLayout.storageBufferLayout(), _shaderLayout.pushConstantsLayout()));
		}
	};
}