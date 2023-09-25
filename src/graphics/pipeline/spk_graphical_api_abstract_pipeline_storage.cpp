#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{
	AbstractPipeline::Object::Storage::Storage(const Storage::Configuration& p_storageConfiguration) :
		_configuration(p_storageConfiguration)
	{
		
	}

	void AbstractPipeline::Object::Storage::clear()
	{
		_content.clear();
	}
	
	const void* AbstractPipeline::Object::Storage::data() const
	{
		return (_content.data());
	}
	
	const size_t AbstractPipeline::Object::Storage::size() const
	{
		return (_content.size());
	}

	AbstractPipeline::Object::Storage::Configuration::Configuration()
	{

	}

	AbstractPipeline::Object::Storage::Configuration::Configuration(const Mode& p_mode)
	{
		mode = p_mode;
		stride = 0;
	}

	AbstractPipeline::Object::Storage::Configuration::Configuration(const Mode& p_mode, const std::map<std::wstring, Configuration::Attribute>& p_attributes)
	{
		static const std::map<AbstractPipeline::Object::Storage::Configuration::Attribute::Type, size_t> typeToSize = {
			{AbstractPipeline::Object::Storage::Configuration::Attribute::Type::Float, sizeof(float)},
			{AbstractPipeline::Object::Storage::Configuration::Attribute::Type::UInt, sizeof(unsigned int)},
			{AbstractPipeline::Object::Storage::Configuration::Attribute::Type::Int, sizeof(int)}
		}; 
		
		mode = p_mode;
		for (const auto& attribute : p_attributes)
		{
			stride += attribute.second.format * typeToSize.at(attribute.second.type);
		}
		attributes = p_attributes;
	}

	void AbstractPipeline::Object::Storage::Configuration::inverseOffset()
	{
		for (auto& attribute : attributes)
		{
			attribute.second.offset = stride - attribute.second.offset - attribute.second.format * attribute.second.unitSize;
		}
	}
	
	std::wostream& operator<<(std::wostream& p_out, const AbstractPipeline::Object::Storage::Configuration& p_config)
	{
		p_out << L"Stride: " << p_config.stride << std::endl;

		for (const auto& attribute : p_config.attributes) {
			p_out << L"Attribute [" << attribute.first << L"]:" << std::endl;
			p_out << L"    Location: " << attribute.second.location << std::endl;
			p_out << L"    Offset: " << attribute.second.offset << std::endl;
			p_out << L"    Unit: " << [&]() -> std::wstring {
				switch (attribute.second.type) {
				case AbstractPipeline::Object::Storage::Configuration::Attribute::Type::Float:
					return L"Float";
				case AbstractPipeline::Object::Storage::Configuration::Attribute::Type::Int:
					return L"Int";
				case AbstractPipeline::Object::Storage::Configuration::Attribute::Type::UInt:
					return L"UInt";
				}
				return L"Unknown"; // This should never happen
				}() << std::endl;
				p_out << L"    Format: " << attribute.second.format << " unit(s)" << std::endl;
		}
		return p_out;
	}
}