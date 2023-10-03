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
	p_os << L"\t\tLocation : " << p_field.location << std::endl;
	p_os << L"\t\tOffset : " << p_field.offset << std::endl;
	p_os << L"\t\tAttribute : " << p_field.attribute;
	return p_os;
	}

	std::wostream& operator<<(std::wostream& p_os, const AbstractPipeline::Configuration::UniformBlockLayout::Field& p_field)
	{
	p_os << L"\t\tName : " << p_field.name << std::endl;
	p_os << L"\t\tOffset : " << p_field.offset << std::endl;
	p_os << L"\t\tAttribute : " << p_field.attribute;
	return p_os;
	}

	std::wostream& operator<<(std::wostream& p_os, const AbstractPipeline::Configuration::UniformBlockLayout& p_block)
	{
	p_os << L"\tname: " << p_block.name << std::endl;
	p_os << L"\ttype: " << p_block.type << std::endl;
	p_os << L"\tkey: " << p_block.set << L" / " << p_block.binding << std::endl;
	p_os << L"\tstride: " << p_block.stride << std::endl;
	p_os << L"\tfields:[" << std::endl;

	for (auto it = p_block.fields.begin(); it != p_block.fields.end(); ++it)
	{
		if (it != p_block.fields.begin())
			p_os << std::endl;
		p_os << *it << std::endl;
	}

	p_os << L"\t]\n";
	p_os << L"}";
	return p_os;
	}

	std::wostream& operator<<(std::wostream& p_os, const AbstractPipeline::Configuration::PushConstantLayout::Field& p_field)
	{
	p_os << L"\t\tName : " << p_field.name << std::endl;
	p_os << L"\t\tOffset : " << p_field.offset << std::endl;
	p_os << L"\t\tAttribute : " << p_field.attribute;
	return p_os;
	}
	
	std::wostream &operator<<(std::wostream &p_os, const AbstractPipeline::Configuration::PushConstantLayout &p_pushConstantLayout)
	{
		p_os << L"\tname: " << p_pushConstantLayout.name << std::endl;
		p_os << L"\ttype: " << p_pushConstantLayout.type << std::endl;
		p_os << L"\tstride: " << p_pushConstantLayout.stride << std::endl;
		p_os << L"\tfields:[" << std::endl;

		for (auto it = p_pushConstantLayout.fields.begin(); it != p_pushConstantLayout.fields.end(); ++it)
		{
			if (it != p_pushConstantLayout.fields.begin())
				p_os << std::endl;
			p_os << *it << std::endl;
		}

		p_os << L"\t]\n";
		p_os << L"}";
		return p_os;
	}
	
	std::wostream &operator<<(std::wostream &p_os, const AbstractPipeline::Configuration::StorageLayout &p_storageLayout)
	{
		p_os << L"\tstride: " << p_storageLayout.stride << std::endl;
		p_os << L"\tfields:[" << std::endl;

		for (auto it = p_storageLayout.fields.begin(); it != p_storageLayout.fields.end(); ++it)
		{
			if (it != p_storageLayout.fields.begin())
				p_os << std::endl;
			p_os << *it << std::endl;
		}

		p_os << L"\t]\n";
		p_os << L"}";
		return p_os;
	}
}
