#include "graphics/pipeline/spk_shader_layout.hpp"

namespace spk
{
	ShaderLayout::Data::Data() :
		type(Type::Error),
		format(1),
		size(0),
		paddingFormat(1)
	{
		
	}

	ShaderLayout::Data::Data(const Type &p_type, const size_t &p_format, const size_t &p_size, const size_t& p_paddingFormat) :
		type(p_type),
		format(p_format),
		size(p_size),
		paddingFormat(p_paddingFormat)
	{

	}

	ShaderLayout::Data::Data(const size_t &p_size, const size_t& p_paddingFormat) : 
		type(Type::Structure),
		format(1),
		size(p_size),
		paddingFormat(p_paddingFormat)
	{

	}

	std::wostream& operator<<(std::wostream& p_out, const ShaderLayout::Data::Type& p_type)
	{
		switch (p_type)
		{
			case ShaderLayout::Data::Type::Int:
				p_out << L"Int";
				break;
			case ShaderLayout::Data::Type::UInt:
				p_out << L"UInt";
				break;
			case ShaderLayout::Data::Type::Float:
				p_out << L"Float";
				break;
			case ShaderLayout::Data::Type::Structure:
				p_out << L"Structure";
				break;
			case ShaderLayout::Data::Type::Error:
				p_out << L"Error";
				break;
			default:
				p_out << L"Unexpected type";
				break;
		}
		return (p_out);
	}

	std::wostream& operator<<(std::wostream& p_out, const ShaderLayout::Data& p_data)
	{
		p_out << p_data.format << " x " << p_data.type << "(" << p_data.size << " bytes)";
		return p_out;
	}
}
