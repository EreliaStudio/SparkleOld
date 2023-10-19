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
			typedef std::basic_ostream<wchar_t, std::char_traits<wchar_t>> wostream;
			
			bool* _needUpdate;
			std::wstring _name;
			uint8_t *_data;
			size_t _offset;
			size_t _size;

		public:
			Field() :
				_needUpdate(nullptr),
				_name(L"Unnamed"),
				_data(nullptr),
				_offset(0),
				_size(0)
			{
			}

			Field(const std::wstring &p_name, bool* p_needUpdate, uint8_t *p_data, size_t p_offset, size_t p_size) :
				_needUpdate(p_needUpdate),
				_name(p_name),
				_data(p_data),
				_offset(p_offset),
				_size(p_size)
			{
			}

			template <typename TType>
			Field &operator<<(const TType &p_value)
			{
				if (sizeof(TType) != _size)
				{
					spk::throwException(L"Field [" + _name + L"] expected a size of [" + std::to_wstring(_size) + L"] and been provided with a structure of size [" + std::to_wstring(sizeof(TType)) + L"]");
				}
				std::memcpy(_data, &p_value, sizeof(TType));
				return (*this);
			}

			Field& operator<<(wostream& (*func)(wostream&)) {
				if (_needUpdate != nullptr)
					*_needUpdate = true;
				return *this;
			}
		};

	private:
		typedef std::basic_ostream<wchar_t, std::char_traits<wchar_t>> wostream;
			
		bool _needUpdate;
		spk::DataBuffer _data;
		std::map<std::wstring, Field> _fields;

	public:
		FieldMap(size_t p_bufferSize) : _data(p_bufferSize)
		{

		}

		void insertNewField(const std::wstring &p_fieldName, size_t p_offset, size_t p_size)
		{
			_fields[p_fieldName] = Field(p_fieldName, &_needUpdate, _data.data(), p_offset, p_size);
		}

		bool needUpdate() const
		{
			return (_needUpdate);
		}

		void setUpdateStatus(bool p_state)
		{
			_needUpdate = p_state;
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

		FieldMap& operator<<(wostream& (*func)(wostream&)) {
			_needUpdate = true;
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