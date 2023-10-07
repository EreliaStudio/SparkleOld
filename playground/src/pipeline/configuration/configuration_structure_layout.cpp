#include "pipeline/pipeline.hpp"

AbstractPipeline::Configuration::StructureLayout::StructureLayout()
{
}

void AbstractPipeline::Configuration::StructureLayout::reset()
{
	_structures = {
		{"float", Data(Data::Type::Float, 1, sizeof(float))},
		{"int", Data(Data::Type::Int, 1, sizeof(int))},
		{"uint", Data(Data::Type::UInt, 1, sizeof(unsigned int))},

		{"vec2", Data(Data::Type::Float, 2, sizeof(float))},
		{"ivec2", Data(Data::Type::Int, 2, sizeof(int))},
		{"uvec2", Data(Data::Type::UInt, 2, sizeof(unsigned int))},

		{"vec3", Data(Data::Type::Float, 3, sizeof(float))},
		{"ivec3", Data(Data::Type::Int, 3, sizeof(int))},
		{"uvec3", Data(Data::Type::UInt, 3, sizeof(unsigned int))},

		{"vec4", Data(Data::Type::Float, 4, sizeof(float))},
		{"ivec4", Data(Data::Type::Int, 4, sizeof(int))},
		{"uvec4", Data(Data::Type::UInt, 4, sizeof(unsigned int))},

		{"mat4", Data(Data::Type::Float, 16, sizeof(float))},
	};

	_standaloneStructures = {
		{"sampler1D", Data(Data::Type::Int, 1, sizeof(int))},
		{"sampler2D", Data(Data::Type::Int, 1, sizeof(int))},
		{"sampler3D", Data(Data::Type::Int, 1, sizeof(int))},
		{"samplerCube", Data(Data::Type::Int, 1, sizeof(int))},
	};
}

void AbstractPipeline::Configuration::StructureLayout::treat(const ShaderModule::Instruction &p_instruction)
{
}

const std::map<std::string, AbstractPipeline::Configuration::Data> &AbstractPipeline::Configuration::StructureLayout::structures() const
{
	return (_structures);
}

const std::map<std::string, AbstractPipeline::Configuration::Data> &AbstractPipeline::Configuration::StructureLayout::standaloneStructures() const
{
	return (_standaloneStructures);
}