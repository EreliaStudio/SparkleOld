#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	AbstractPipeline::FieldMap::Field::Field()
		: _data(nullptr), _offset(0), _size(0)
	{
	}

	AbstractPipeline::FieldMap::Field::Field(uint8_t *p_data, const size_t& p_offset, const size_t& p_size)
		: _data(p_data), _offset(p_offset), _size(p_size)
	{
	}

	void AbstractPipeline::FieldMap::_addField(const std::wstring& p_name, const size_t& p_offset, const size_t& p_size)
	{
		_fields[p_name] = Field(_data.data(), p_offset, p_size);
	}

	AbstractPipeline::FieldMap::FieldMap(const std::wstring& p_name, size_t p_dataSize) :
		_name(p_name),
		_data()
	{
		_data.resize(p_dataSize);
	}

	const void* AbstractPipeline::FieldMap::data() const
	{
		return _data.data();
	}
	
	size_t AbstractPipeline::FieldMap::size() const
	{
		return _data.size();
	}

	bool AbstractPipeline::FieldMap::contains(const std::wstring& p_fieldName)
	{
		return (_fields.contains(p_fieldName));
	}
	
	AbstractPipeline::FieldMap::Field& AbstractPipeline::FieldMap::field(const std::wstring& p_fieldName)
	{
		if (!contains(p_fieldName))
		{
			spk::throwException(L"Field not found: " + p_fieldName);
		}
		return _fields[p_fieldName];
	}
}
