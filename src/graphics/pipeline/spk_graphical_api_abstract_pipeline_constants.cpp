#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"

namespace spk::GraphicalAPI
{
	AbstractPipeline::Object::Constants::Configuration::Structure::Attribute::Attribute()
	{
	}

	AbstractPipeline::Object::Constants::Configuration::Structure::Attribute::Attribute(
		const size_t &p_format,
		const Object::Constants::Configuration::Structure::Attribute::Type &p_type,
		const size_t &p_unitSize,
		const size_t &p_offset) :
			format(p_format),
			type(p_type),
			unitSize(p_unitSize),
			offset(p_offset)
	{
	}

	AbstractPipeline::Object::Constants::Configuration::Structure::Structure()
	{
	}

	AbstractPipeline::Object::Constants::Configuration::Structure::Structure(const Object::Constants::Configuration::Structure::Attribute &p_attribute)
	{
		size = p_attribute.unitSize * p_attribute.format;
		attributes[""] = p_attribute;
	}

	AbstractPipeline::Object::Constants::Configuration::Configuration() :
		stride(0),
		structures({
			{"int", Structure::Attribute(1, Structure::Attribute::Type::Int, sizeof(int), 0)},
			{"uint", Structure::Attribute(1, Structure::Attribute::Type::UInt, sizeof(unsigned int), 0)},
			{"float", Structure::Attribute(1, Structure::Attribute::Type::Float, sizeof(float), 0)},

			{"ivec2", Structure::Attribute(2, Structure::Attribute::Type::Int, sizeof(int), 0)},
			{"uvec2", Structure::Attribute(2, Structure::Attribute::Type::UInt, sizeof(unsigned int), 0)},
			{"vec2", Structure::Attribute(2, Structure::Attribute::Type::Float, sizeof(float), 0)},

			{"ivec3", Structure::Attribute(3, Structure::Attribute::Type::Int, sizeof(int), 0)},
			{"uvec3", Structure::Attribute(3, Structure::Attribute::Type::UInt, sizeof(unsigned int), 0)},
			{"vec3", Structure::Attribute(3, Structure::Attribute::Type::Float, sizeof(float), 0)},

			{"ivec4", Structure::Attribute(4, Structure::Attribute::Type::Int, sizeof(int), 0)},
			{"uvec4", Structure::Attribute(4, Structure::Attribute::Type::UInt, sizeof(unsigned int), 0)},
			{"vec4", Structure::Attribute(4, Structure::Attribute::Type::Float, sizeof(float), 0)},

			{"mat4", Structure::Attribute(16, Structure::Attribute::Type::Float, sizeof(float), 0)},

			{"sampler1D", Structure::Attribute(1, Structure::Attribute::Type::Int, sizeof(int), 0)},
			{"sampler2D", Structure::Attribute(1, Structure::Attribute::Type::Int, sizeof(int), 0)},
			{"sampler3D", Structure::Attribute(1, Structure::Attribute::Type::Int, sizeof(int), 0)},
			{"samplerCube", Structure::Attribute(1, Structure::Attribute::Type::Int, sizeof(int), 0)},
		})
	{
	}

	std::wostream &operator<<(std::wostream &p_out, const AbstractPipeline::Object::Constants::Configuration &p_config)
	{
		p_out << L"Stride: " << p_config.stride << std::endl;

		for (const auto &structurePair : p_config.structures)
		{
			const auto &structureName = structurePair.first;
			const auto &structure = structurePair.second;

			p_out << L"\tStructure [" << spk::to_wstring(structureName) << L"]:" << std::endl;
			p_out << L"\t\tSize [" << structure.size << L"]:" << std::endl;
			for (const auto &attributePair : structure.attributes)
			{
				const auto &attributeName = attributePair.first;
				const auto &attribute = attributePair.second;

				p_out << L"\t\tAttribute [" << spk::to_wstring(attributeName) << L"]:" << std::endl;
				p_out << L"\t\t\tFormat: " << attribute.format << std::endl;
				p_out << L"\t\t\tType: ";

				switch (attribute.type)
				{
				case AbstractPipeline::Object::Constants::Configuration::Structure::Attribute::Type::Int:
					p_out << L"Int";
					break;
				case AbstractPipeline::Object::Constants::Configuration::Structure::Attribute::Type::UInt:
					p_out << L"UInt";
					break;
				case AbstractPipeline::Object::Constants::Configuration::Structure::Attribute::Type::Float:
					p_out << L"Float";
					break;
				}
				p_out << std::endl;

				p_out << L"\t\t\tUnit Size: " << attribute.unitSize << std::endl;
				p_out << L"\t\t\tOffset: " << attribute.offset << std::endl;
			}
		}
		return p_out;
	}
}