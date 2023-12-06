#include "data_structure/spk_field_map.hpp"
#include <stdexcept>  // pour std::runtime_error

namespace spk {
    FieldMap::Field::Field() :
        _needUpdate(nullptr),
        _name(L"Unnamed"),
        _data(nullptr),
        _offset(0),
        _size(0)
    {
    }

    FieldMap::Field::Field(const std::wstring& p_name, bool* p_needUpdate, uint8_t* p_data, size_t p_offset, size_t p_size) :
        _needUpdate(p_needUpdate),
        _name(p_name),
        _data(p_data),
        _offset(p_offset),
        _size(p_size)
    {
    }
	
    const std::wstring& FieldMap::Field::name() const
    {
        return (_name);
    }
    
    const size_t& FieldMap::Field::size() const
    {
        return (_size);
    }
    
    const size_t& FieldMap::Field::offset() const
    {
        return (_offset);
    }

	FieldMap::Field& FieldMap::Field::operator<<(wostream& (*func)(wostream&)) {
		if (_needUpdate != nullptr)
			*_needUpdate = true;
		return *this;
	}
	
	FieldMap::FieldMap(size_t p_bufferSize) : _needUpdate(false), _data(p_bufferSize)
    {
    }

    const std::map<std::wstring, FieldMap::Field>& FieldMap::fields() const
    {
        return (_fields);
    }

    void FieldMap::insertNewField(const std::wstring& p_fieldName, size_t p_offset, size_t p_size)
    {
        _fields[p_fieldName] = Field(p_fieldName, &_needUpdate, _data.data(), p_offset, p_size);
    }

    bool FieldMap::needUpdate() const
    {
        return _needUpdate;
    }

    void FieldMap::setUpdateStatus(bool p_state)
    {
        _needUpdate = p_state;
    }

    FieldMap::Field& FieldMap::field(const std::wstring& p_fieldName)
    {
        if (_fields.find(p_fieldName) == _fields.end())
        {
            throw std::runtime_error("Field does not exist");
        }
        return _fields[p_fieldName];
    }

    FieldMap::Field& FieldMap::operator[](const std::wstring& p_fieldName)
    {
        return this->field(p_fieldName);
    }

    const uint8_t* FieldMap::data() const
    {
        return _data.data();
    }

    size_t FieldMap::size() const
    {
        return _data.size();
    }

	FieldMap& FieldMap::operator<<(wostream& (*func)(wostream&)) {
		_needUpdate = true;
		return *this;
	}
}
