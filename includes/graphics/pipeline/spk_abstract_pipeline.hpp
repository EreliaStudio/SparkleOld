#pragma once

#include "data_structure/spk_data_buffer.hpp"
#include "data_structure/spk_field_map.hpp"
#include "graphics/pipeline/spk_pipeline_basic_functions.hpp"
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
					typedef std::basic_ostream<wchar_t, std::char_traits<wchar_t>> wostream;
					bool _needUpdate;
					std::wstring _name;
					size_t _unitSize;
					spk::DataBuffer _data;

				public:
					Buffer(const std::wstring &p_name, size_t p_unitSize) : _unitSize(p_unitSize),
																			_name(p_name)
					{
					}

					void clear()
					{
						_data.clear();
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

					Buffer& operator<<(wostream& (*func)(wostream&)) {
						_needUpdate = true;
						return *this;
					}

					void setUpdateStatus(bool p_state)
					{
						_needUpdate = p_state;
					}

					bool needUpdate() const
					{
						return (_needUpdate);
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
					_indexes(L"Indexes", sizeof(uint32_t))
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

			void _updateVertices()
			{
				_pushVerticesData(_storage.vertices().data(), _storage.vertices().size());
				_storage.vertices().clear();
				_storage.vertices().setUpdateStatus(false);
			}

			void _updateIndexes()
			{
				_pushIndexesData(_storage.indexes().data(), _storage.indexes().size());
				_nbIndexesPushed = _storage.indexes().size() / sizeof(size_t);
				_storage.indexes().clear();
				_storage.indexes().setUpdateStatus(false);
			}

			void _updatePushConstants()
			{
				_pushPushConstantsData(_pushConstants.data(), _pushConstants.size());
				_pushConstants.setUpdateStatus(false);
			}

		public:
			Object(AbstractPipeline* p_owner, const spk::ShaderLayout::StorageBufferLayout& p_storageBufferLayout, const spk::ShaderLayout::PushConstantsLayout& p_pushConstantsLayout) :
				_owner(p_owner),
				_storage(p_storageBufferLayout),
				_pushConstants(p_pushConstantsLayout),
				_nbIndexesPushed(0)
			{

			}

			void render()
			{
				_owner->activate();

				if (_storage.vertices().needUpdate() == true)
					_updateVertices();
				if (_storage.indexes().needUpdate() == true)
					_updateIndexes();
				if (_pushConstants.needUpdate() == true)
					_updatePushConstants();

				_onRender();

				spk::cout << "NbIndexPushed : " << _nbIndexesPushed << std::endl;
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

		class Uniform
		{
		protected:
			std::wstring _name;
			spk::DataBuffer _data;

			virtual void _pushData() = 0;

		public:
			Uniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) :
				_data(p_uniformBlockLayout.stride()),
				_name(p_uniformBlockLayout.name())
			{

			}

			const uint8_t *data() const
			{
				return (_data.data());
			}

			size_t size() const
			{
				return (_data.size());
			}
			
			template <typename TType>
			Uniform &operator<<(const TType &p_data)
			{
				if (_data.size() != sizeof(TType))
				{
					spk::throwException(L"Pushing an unexpected data with size [" + std::to_wstring(sizeof(TType)) + L"] into a uniform named [" + _name + L"] of size [" + std::to_wstring(_data.size()) + L"]");
				}
				_data.edit(0, p_data);
				return (*this);
			}

			Uniform& operator<<(std::wostream& (*func)(std::wostream&)) {
				_pushData();
				return *this;
			}
		};

		class UniformBlock : public Uniform
		{
		public:
			class Field
			{
			private:
				std::wstring _name;
				UniformBlock *_owner;
				void* _data;
				size_t _offset;
				size_t _size;

			public:
				Field() :
					_owner(nullptr),
					_name(L"Unnamed"),
					_data(nullptr),
					_offset(0),
					_size(0)
				{
				}

				Field(const std::wstring& p_name, UniformBlock *p_owner, uint8_t *p_data, size_t p_offset, size_t p_size) :
					_name(p_name),
					_owner(p_owner),
					_data(p_data),
					_offset(p_offset),
					_size(p_size)
				{
				}

				template <typename TType>
				Field& operator<<(const TType& p_data)
				{
					if (_size != sizeof(TType))
					{
						spk::throwException(L"Incorrect size of data pushed to the field [" + _name + L"].");
					}

					std::memcpy(static_cast<uint8_t*>(_data) + _offset, &p_data, _size);
					return *this;
				}
					
				Field& operator<<(std::wostream& (*func)(std::wostream&))
				{
					if (_owner != nullptr)
						_owner->_launchPushData();
					return *this;
				}
			};

		private:
    		std::map<std::wstring, Field> _fields;

			void _launchPushData()
			{
				_pushData();
			}

		public:
			UniformBlock(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) :
				Uniform(p_uniformBlockLayout)
			{
				for (const auto& field : p_uniformBlockLayout.fields())
				{
					_fields.emplace(
						spk::to_wstring(field.name),
						Field(
							p_uniformBlockLayout.name() + L"::" + spk::to_wstring(field.name),
							this,
							_data.data(),
							field.offset,
							field.data.format * field.data.size
						)
					);
				}
			}

			Field &field(const std::wstring &p_fieldName)
			{
				if (_fields.contains(p_fieldName) == false)
					spk::throwException(L"Field [" + p_fieldName + L"] doesn't exist in FieldMap");
				return (_fields[p_fieldName]);
			}

			Field &operator[](const std::wstring &p_fieldName)
			{
				return (this->field(p_fieldName));
			}
		};

		class SamplerUniform : public Uniform
		{
		private:

		public:
			SamplerUniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) :
				Uniform(p_uniformBlockLayout)
			{

			}
		};

	protected:
		ShaderLayout _shaderLayout;

		static inline std::map<ShaderLayout::UniformBlockLayout::Key, std::shared_ptr<Uniform>> _uniformMap;
		std::map<std::wstring, ShaderLayout::UniformBlockLayout::Key> _uniformKeys;

		virtual void _loadProgram(const ShaderLayout &p_shaderLayout) = 0;

		std::shared_ptr<Uniform> _loadUniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout)
		{
			switch (p_uniformBlockLayout.mode())
			{
			case ShaderLayout::UniformBlockLayout::Mode::Block:
				return (_loadUniformBlock(p_uniformBlockLayout));
			case ShaderLayout::UniformBlockLayout::Mode::Single:
				return (_loadSamplerUniform(p_uniformBlockLayout));
			}
			return (nullptr);
		}

		void _loadPipeline()
		{
			_loadProgram(_shaderLayout);

			for (const auto& uniformBlockLayout : _shaderLayout.uniformBlockLayouts())
			{
				if (_uniformMap.contains(uniformBlockLayout.key()) == false)
				{
					std::shared_ptr<Uniform> newUniform = _loadUniform(uniformBlockLayout);

					_uniformMap[uniformBlockLayout.key()] = newUniform;
				}

				std::shared_ptr<Uniform> targetUniform = _uniformMap[uniformBlockLayout.key()];

				if (targetUniform->size() != uniformBlockLayout.stride())
					spk::throwException(L"Uniform named [" + uniformBlockLayout.name() + L"] collide with an existing uniform of different size, at set [" + std::to_wstring(uniformBlockLayout.key().set) + L"] and binding [" + std::to_wstring(uniformBlockLayout.key().binding) + L"]");
				
				_uniformKeys[uniformBlockLayout.name()] = uniformBlockLayout.key();
			}
		}

		virtual std::shared_ptr<UniformBlock> _loadUniformBlock(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) = 0;
		virtual std::shared_ptr<SamplerUniform> _loadSamplerUniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) = 0;
		virtual std::shared_ptr<Object> _loadObject(const ShaderLayout::StorageBufferLayout& p_storageLayout, const ShaderLayout::PushConstantsLayout& p_pushConstantsLayout) = 0;

	public:
		AbstractPipeline(const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput) : _shaderLayout(p_vertexInput, p_fragmentInput)
		{

		}

		virtual void launch(const size_t &p_nbIndexes) = 0;

		virtual void activate() = 0;
		virtual void deactivate() = 0;

		Uniform& uniform(const std::wstring& p_uniformName)
		{
			if (_uniformKeys.contains(p_uniformName) == false)
				spk::throwException(L"Uniform named [" + p_uniformName + L"] doesn't exist.");
			ShaderLayout::UniformBlockLayout::Key uniformKey = _uniformKeys[p_uniformName];

			if (_uniformMap.contains(uniformKey) == false)
				spk::throwException(L"Uniform named [" + p_uniformName + L"] doesn't link to an existing key [" + std::to_wstring(uniformKey.set) + L" / " + std::to_wstring(uniformKey.binding) + L"].");
		
			return (*(_uniformMap[uniformKey]));
		}

		std::shared_ptr<Object> createObject()
		{
			return (_loadObject(_shaderLayout.storageBufferLayout(), _shaderLayout.pushConstantsLayout()));
		}
	};
}