#pragma once

#include "data_structure/spk_data_buffer.hpp"

#include <string>
#include <map>

#include "spk_basic_functions.hpp"

namespace spk
{
    /**
    * Class that manages a map of fields.
    */
	class FieldMap
	{
	public:
        /**
        * Nested class that represents an individual field within the FieldMap.
        */
		class Field
		{
			friend class FieldMap;

		private:
            /**
            * Alias for wide character output stream.
            */
			typedef std::basic_ostream<wchar_t, std::char_traits<wchar_t>> wostream;

            /**
            * Flag indicating whether this field needs to be updated.
            */
			bool* _needUpdate;

            /**
            * Name of the field.
            */
			std::wstring _name;

            /**
            * Raw data for this field.
            */
			uint8_t *_data;

            /**
            * Offset where this field's data starts.
            */
			size_t _offset;

            /**
            * Size of this field.
            */
			size_t _size;

		public:
            /**
            * Default constructor.
            */
			Field();

            /**
            * Overloaded constructor.
            */
			Field(const std::wstring &p_name, bool* p_needUpdate, uint8_t *p_data, size_t p_offset, size_t p_size);

            /**
            * Overload insertion operator to insert typed data into this field.
            */
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
            
            /**
            * Overload insertion operator to insert typed data into this field.
            */
			template <typename TType>
			Field &operator = (const TType &p_value)
			{
				if (sizeof(TType) != _size)
				{
					spk::throwException(L"Field [" + _name + L"] expected a size of [" + std::to_wstring(_size) + L"] and been provided with a structure of size [" + std::to_wstring(sizeof(TType)) + L"]");
				}
				std::memcpy(_data, &p_value, sizeof(TType));
                if (_needUpdate != nullptr)
                    *_needUpdate = true;
				return (*this);
			}

            /**
            * Overload insertion operator for functions.
            */
			Field& operator<<(wostream& (*func)(wostream&));
		};

	private:
        /**
        * Alias for wide character output stream.
        */
		typedef std::basic_ostream<wchar_t, std::char_traits<wchar_t>> wostream;

        /**
        * Flag indicating whether this FieldMap needs to be updated.
        */
		bool _needUpdate;

        /**
        * Buffer that contains the data for all fields.
        */
		spk::DataBuffer _data;

        /**
        * Map of field names to Field objects.
        */
		std::map<std::wstring, Field> _fields;

	public:
        /**
        * Constructor that initializes the buffer size.
        */
		FieldMap(size_t p_bufferSize);

        /**
        * Inserts a new field into the map.
        */
		void insertNewField(const std::wstring &p_fieldName, size_t p_offset, size_t p_size);

        /**
        * Check whether this FieldMap needs to be updated.
        */
		bool needUpdate() const;

        /**
        * Sets the update status of this FieldMap.
        */
		void setUpdateStatus(bool p_state);

        /**
        * Overload insertion operator to insert typed data into the FieldMap.
        */
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

        /**
        * Overload insertion operator to insert typed data into the FieldMap.
        */
		template <typename TType>
		FieldMap &operator = (const TType &p_value)
		{
			if (sizeof(TType) != _data.size())
			{
				spk::throwException(L"Unexpected structure size to push inside a FieldMap\nExpected a size of [" + std::to_wstring(_data.size()) + L"] and been provided with a structure of size [" + std::to_wstring(sizeof(TType)) + L"]");
			}
			std::memcpy(_data.data(), &p_value, sizeof(TType));
            _needUpdate = true;
			return *this;
		}

        /**
        * Overload insertion operator for functions.
        */
		FieldMap& operator<<(wostream& (*func)(wostream&));

        /**
        * Returns a reference to a field given its name.
        */
		Field &field(const std::wstring &p_fieldName);

        /**
        * Overload of subscript operator for convenient field access.
        */
		Field &operator[](const std::wstring &p_fieldName);

        /**
        * Returns a pointer to the raw data of the FieldMap.
        */
		const uint8_t *data() const;

        /**
        * Returns the total size of the FieldMap's data.
        */
		size_t size() const;
	};
}