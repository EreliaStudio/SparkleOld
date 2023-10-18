#pragma once

#include "data_structure/spk_data_buffer.hpp"

#include <string>
#include <map>

#include "spk_basic_functions.hpp"

namespace spk
{
	class FieldMap
	{
	public:
		class Field
		{
			friend class FieldMap;

		private:
			std::wstring name;
			uint8_t *data;
			size_t offset;
			size_t size;

		public:
			Field() :
				name(L"Unnamed"),
				data(nullptr),
				offset(0),
				size(0)
			{
			}

			Field(const std::wstring &p_name, uint8_t *p_data, size_t p_offset, size_t p_size) :
				name(p_name),
				data(p_data),
				offset(p_offset),
				size(p_size)
			{
			}

			template <typename TType>
			Field &operator<<(const TType &p_value)
			{
				if (sizeof(TType) != size)
				{
					spk::throwException(L"Field [" + name + L"] expected a size of [" + std::to_wstring(size) + L"] and been provided with a structure of size [" + std::to_wstring(sizeof(TType)) + L"]");
				}
				std::memcpy(data, &p_value, sizeof(TType));
				return (*this);
			}
		};

	private:
		spk::DataBuffer _data;
		std::map<std::wstring, Field> _fields;

	public:
		FieldMap(size_t p_bufferSize) : _data(p_bufferSize)
		{

		}

		void insertNewField(const std::wstring &p_fieldName, size_t p_offset, size_t p_size)
		{
			_fields[p_fieldName] = Field(p_fieldName, _data.data(), p_offset, p_size);
		}

		template <typename TType>
		FieldMap &operator<<(const TType &p_value)
		{
			if (sizeof(TType) != _data.size())
			{
				spk::throwException(L"Unexpected structure size to push inside a FieldMap\nExpected a size of [" + std::to_wstring(_data.size()) + L"] and been provided with a structure of size [" + std::to_wstring(sizeof(TType)) + L"]");
			}
			std::memcpy(_data.data(), &p_value, sizeof(TType));
			return *this;
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

		const uint8_t *data() const
		{
			return (_data.data());
		}

		size_t size() const
		{
			return (_data.size());
		}
	};
}