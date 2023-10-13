#pragma once

#include "data_structure/spk_data_buffer.hpp"
#include "graphics/pipeline/spk_shader_module.hpp"
#include "graphics/pipeline/spk_shader_layout.hpp"
#include <map>

#include "spk_basic_functions.hpp"

namespace spk
{
	class AbstractPipeline
	{
	private:
	
			class FieldMap
			{
			public:
				class Field
				{
				private:
					uint8_t *_data;
					size_t _offset;
					size_t _size;

				public:
					Field();
					Field(uint8_t *p_data, const size_t& p_offset, const size_t& p_size);

					template<typename TType>
					Field& operator << (const TType& p_input)
					{
						if (sizeof(TType) != _size)
							spk::throwException(L"Unexpected data size [" + std::to_wstring(sizeof(TType)) + L"] pushed into a [" + std::to_wstring(_size) + L"] octets field");
						std::memcpy(_data + _offset, &p_input, sizeof(TType));
					}
				};
			private:
				std::wstring _name;
				spk::DataBuffer _data;
				std::map<std::wstring, Field> _fields;

			protected:
				void _addField(const std::wstring& p_name, const size_t& p_offset, const size_t& p_size);

			public:
				FieldMap(const std::wstring& p_name, size_t p_dataSize);

				const void* data() const;
				size_t size() const;

				template<typename TType>
				FieldMap& operator << (const TType& p_input)
				{	
					if (sizeof(TType) != _data.size())
					{
						spk::throwException(L"Can't push a data of size [" + spk::to_wstring(sizeof(TType)) + L"] into a " + _name + L" of size [" + spk::to_wstring(_data.size()) + L"]");
					}

					std::memcpy(_data.data(), &p_input, sizeof(TType));
				}
				bool contains(const std::wstring& p_fieldName);
				Field& field(const std::wstring& p_fieldName);
			};

	public:
		class Object
		{
			friend class AbstractPipeline;

		public:
			class Storage
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
				size_t _stride;
				spk::DataBuffer _data;

			public:
				Storage(const ShaderLayout::StorageBufferLayout& p_layout);
				void clear();

				template <typename... Types>
				Storage &operator<<(const Unit<Types...> &p_unit)
				{
					if (sizeof(p_unit) != _stride)
						spk::throwException(L"Unexpected unit size [" + std::to_wstring(sizeof(p_unit)) + L"] vs expected [" + std::to_wstring(_stride) + L"]");
					_data.append(&p_unit, sizeof(p_unit));

					return (*this);
				}

				template <typename... Types>
				Storage &operator<<(const std::vector<Unit<Types...>> &p_unitVector)
				{
					if (sizeof(p_unitVector[0]) != _stride)
						spk::throwException(L"Unexpected unit size [" + std::to_wstring(sizeof(p_unitVector[0])) + L"] vs expected [" + std::to_wstring(_stride) + L"]");
					_data.append(p_unitVector.data(), sizeof(Unit<Types...>) * p_unitVector.size());

					return (*this);
				}

				const void* data() const;
				size_t size() const;
			};

			class PushConstants : public FieldMap
			{
			private:

			public:
				PushConstants(const ShaderLayout::PushConstantLayout& p_pushConstantsLayout);
			};

		protected:
			AbstractPipeline *_owner;
			Storage _storage;
			PushConstants _pushConstants;

			virtual void _pushStorageDatas(const void *p_data, size_t p_size) = 0;
			virtual void _pushConstantsDatas(const void *p_data, size_t p_size) = 0;

			Object(AbstractPipeline *p_owner, const ShaderLayout::StorageBufferLayout& p_storageLayout, const ShaderLayout::PushConstantLayout& p_pushConstantsLayout);

		public:			

			Object(const Object& p_object) = delete;
			Object& operator=(const Object& p_other) = delete;

			Object(Object&& p_other) noexcept;
			Object& operator=(Object&& p_other) noexcept;

			virtual void render() = 0;

			void updateStorage();
			void updateConstants();

			Storage& storage();
			PushConstants& pushConstants();
		};

		class Uniform
		{
		private:

		public:
			virtual void update() = 0;
		};

		class UniformBlock : public Uniform, public FieldMap
		{
		private:
			virtual void _updateData(const void* p_data, size_t p_dataSize) = 0;

		public:
			UniformBlock(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout);
			void update();

			virtual void activate() = 0;
			virtual void deactivate() = 0;
		};

		class SamplerUniform : public Uniform
		{
		private:
			int _samplerID;

			virtual void _updateSamplerID(int p_samplerID) = 0;

		public:
			SamplerUniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout);
			void update();

			SamplerUniform& operator=(int p_samplerID);
			void set(int p_samplerID);

			virtual void activate() = 0;
			virtual void deactivate() = 0;
		};

	protected:
		ShaderLayout _shaderLayout;

		static inline std::map<ShaderLayout::UniformBlockLayout::Key, std::shared_ptr<Uniform>> _uniformBlocks;
		static inline std::map<std::wstring, ShaderLayout::UniformBlockLayout::Key> _uniformBlocksKeys;
		
		void addUniformBlock(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout);

		virtual std::shared_ptr<UniformBlock> _loadUniformBlock(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) = 0;
		virtual std::shared_ptr<SamplerUniform> _loadSamplerUniform(const ShaderLayout::UniformBlockLayout& p_uniformBlockLayout) = 0;
		virtual std::shared_ptr<Object> _loadObject(const ShaderLayout::StorageBufferLayout& p_storageLayout, const ShaderLayout::PushConstantLayout& p_pushConstantsLayout) = 0;

		virtual void _loadProgram(const ShaderLayout& p_shaderLayout) = 0;
		void _loadPipeline();

	public:
		AbstractPipeline(const ShaderModule &p_vertexInput, const ShaderModule &p_fragmentInput);

		std::shared_ptr<Object> createObject();
		std::shared_ptr<Uniform> getUniform(const std::wstring& p_uniformBlockName);
	};
}