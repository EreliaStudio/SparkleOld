#include "graphics/pipeline/spk_shader_layout.hpp"

#include "spk_basic_functions.hpp"

namespace spk
{
	ShaderLayout::FieldArrayLayout::FieldArrayLayout(const StructureLayout &p_structureLayout, const size_t& p_stridePadding) :
		structureLayout(p_structureLayout),
		_stride(0),
		_currentOffset(0),
		_stridePadding(p_stridePadding)
	{
	}

	void ShaderLayout::FieldArrayLayout::insert(const std::string &p_name, const Data &p_data, const size_t &p_location)
	{
		Field newField;
		size_t fieldSize = p_data.format * p_data.size;

		newField.name = p_name;
		newField.data = p_data;
		newField.location = p_location;

		if ((_currentOffset % p_data.paddingFormat) != 0)
		{
			_currentOffset += (p_data.paddingFormat - (_currentOffset % p_data.paddingFormat));
		}

		newField.offset = _currentOffset;

		_fields.push_back(newField);
		_currentOffset += fieldSize;
		
		while (_currentOffset > _stride)
		{
			_stride += _stridePadding;
		}
	}

	std::wostream& operator<<(std::wostream& p_out, const ShaderLayout::FieldArrayLayout::Field& p_field)
	{
		p_out << L"\t\t\tName: " << spk::to_wstring(p_field.name) << std::endl;
		p_out << L"\t\t\tData: " << p_field.data << std::endl;
		if (p_field.location != -1)
			p_out << L"\t\t\tLocation: " << p_field.location << std::endl;
		p_out << L"\t\t\tOffset: " << p_field.offset << std::endl;
		return p_out;
	}

	std::wostream& operator<<(std::wostream& p_out, const ShaderLayout::FieldArrayLayout& p_layout)
	{
		p_out << "\t\tStride : " << p_layout._stride << std::endl;
		p_out << "\t\tFields : " << std::endl;
		for (size_t i = 0; i < p_layout.fields().size(); i++)
		{
			if (i != 0)
				p_out << std::endl;
			p_out << p_layout.fields()[i];
		}
		return p_out;
	}

	const size_t &ShaderLayout::FieldArrayLayout::stride() const
	{
		return (_stride);
	}

	const std::vector<ShaderLayout::FieldArrayLayout::Field> &ShaderLayout::FieldArrayLayout::fields() const
	{
		return (_fields);
	}
}