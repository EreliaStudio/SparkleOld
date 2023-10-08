#include "graphics/pipeline/spk_abstract_pipeline.hpp"

namespace spk
{
	AbstractPipeline::Configuration::Data::Data(const Type &p_type, const size_t &p_format, const size_t &p_size) :
		type(p_type),
		format(p_format),
		size(p_size)
	{

	}

	AbstractPipeline::Configuration::Data::Data(const size_t &p_size) : 
		type(Type::Structure),
		format(1),
		size(p_size)
	{

	}

	std::wostream& operator<<(std::wostream& p_out, const AbstractPipeline::Configuration::Data::Type& p_type)
	{
		switch (p_type)
		{
			case AbstractPipeline::Configuration::Data::Type::Int:
				p_out << L"Int";
				break;
			case AbstractPipeline::Configuration::Data::Type::UInt:
				p_out << L"UInt";
				break;
			case AbstractPipeline::Configuration::Data::Type::Float:
				p_out << L"Float";
				break;
			case AbstractPipeline::Configuration::Data::Type::Structure:
				p_out << L"Structure";
				break;
			case AbstractPipeline::Configuration::Data::Type::Error:
				p_out << L"Error";
				break;
			default:
				p_out << L"Unexpected type";
				break;
		}
		return (p_out);
	}

	std::wostream& operator<<(std::wostream& p_out, const AbstractPipeline::Configuration::Data& p_data)
	{
		p_out << p_data.format << " x " << p_data.type << "(" << p_data.size << " bytes)";
		return p_out;
	}
}
