#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{
	std::wostream& operator<<(std::wostream& p_os, const AbstractPipeline::Configuration::Data::Type& p_type)
	{
	p_os << (p_type == AbstractPipeline::Configuration::Data::Type::Error  ? L"Error"  :
			p_type == AbstractPipeline::Configuration::Data::Type::Int	? L"Int":
			p_type == AbstractPipeline::Configuration::Data::Type::UInt   ? L"UInt"   :
			p_type == AbstractPipeline::Configuration::Data::Type::Float  ? L"Float"  :
	  L"Complex");
	return p_os;
	}

	std::wostream& operator<<(std::wostream& p_os, const AbstractPipeline::Configuration::Data& p_data)
	{
	p_os << L"Data{type: " << p_data.type << L", format: " << p_data.format << L", size: " << p_data.size << L"}";
	return p_os;
	}

	std::wostream& operator<<(std::wostream& p_os, const AbstractPipeline::Configuration::StorageLayout::Field& p_field)
	{
	p_os << L"Field{location: " << p_field.location << L", offset: " << p_field.offset << L", attribute: " << p_field.attribute << L"}";
	return p_os;
	}

	std::wostream& operator<<(std::wostream& p_os, const AbstractPipeline::Configuration::UniformBlockLayout::Block::Key& p_key)
	{
	p_os << L"Key{set: " << p_key.set << L", binding: " << p_key.binding << L"}";
	return p_os;
	}

	std::wostream& operator<<(std::wostream& p_os, const AbstractPipeline::Configuration::UniformBlockLayout::Block::Field& p_field)
	{
	p_os << L"\t\tName : " << p_field.name << std::endl;
	p_os << L"\t\tOffset : " << p_field.offset << std::endl;
	p_os << L"\t\tAttribute : " << p_field.attribute;
	return p_os;
	}

	std::wostream& operator<<(std::wostream& p_os, const AbstractPipeline::Configuration::UniformBlockLayout::Block& p_block)
	{
	p_os << L"\tstride: " << p_block.stride << std::endl;
	p_os << L"\tfields:" << std::endl;

	for (auto it = p_block.fields.begin(); it != p_block.fields.end(); ++it)
	{
		p_os << L"" << *it << std::endl;
	}

	p_os << L"\t]\n";
	p_os << L"}";
	return p_os;
	}

	std::wostream& operator<<(std::wostream& p_os, const AbstractPipeline::Configuration::PushConstantLayout::Field& p_field)
	{
	p_os << L"Field{offset: " << p_field.offset << L", attribute: " << p_field.attribute << L"}";
	return p_os;
	}
}
