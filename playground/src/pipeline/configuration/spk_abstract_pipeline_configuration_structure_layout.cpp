#include "pipeline/spk_abstract_pipeline.hpp"

#include <regex>

AbstractPipeline::Configuration::StructureLayout::StructureLayout()
{
	_singleUniformStructures = {
		{"sampler1D", Data(Data::Type::Int, 1, sizeof(int))},
		{"sampler2D", Data(Data::Type::Int, 1, sizeof(int))},
		{"sampler3D", Data(Data::Type::Int, 1, sizeof(int))},
		{"samplerCube", Data(Data::Type::Int, 1, sizeof(int))},
	};

    _acceptedSingleUniformTypeString = L"sampler1D, sampler2D, sampler3D, samplerCube";
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
}


void AbstractPipeline::Configuration::StructureLayout::treat(const ShaderModule::Instruction &p_instruction)
{
    std::regex re(R"(struct\s+(\w+)\s+\{\s*(.*)\s*\};)");
    std::smatch match;
    
    if (std::regex_search(p_instruction.code, match, re))
    {
        std::string structName = match[1];
        std::string fields = match[2];
        
        std::regex field_re(R"((\w+)\s+(\w+)\s*;)");
        std::smatch field_match;
        std::string::const_iterator searchStart(fields.cbegin());

        size_t totalStride = 0;

        while (std::regex_search(searchStart, fields.cend(), field_match, field_re))
        {
            std::string dataType = field_match[1];
            
            auto it = _structures.find(dataType);
            if (it != _structures.end())
            {
                totalStride += it->second.size;
            }
            else
            {
                spk::throwException(L"Unexpected Structure member type [" + spk::to_wstring(dataType) + L"] inside [" + spk::to_wstring(p_instruction.code) + L"]");
            }

            searchStart = field_match.suffix().first;
        }

        Data structData(Data::Type::Structure, 1, totalStride);
        
        _structures[structName] = structData;
    }
    else
    {
        spk::throwException(L"Unexpected Structure instruction [" + spk::to_wstring(p_instruction.code) + L"]\nExpected format :\nstruct [STRUCTURE_NAME]\n{\n    [ValueType] [ValueName];\n    [Repeat for each attribute];\n};");
    }
}

const std::wstring& AbstractPipeline::Configuration::StructureLayout::acceptedSingleUniformTypeString() const
{
    return (_acceptedSingleUniformTypeString);
}

const std::map<std::string, AbstractPipeline::Configuration::Data> &AbstractPipeline::Configuration::StructureLayout::structures() const
{
	return (_structures);
}

const std::map<std::string, AbstractPipeline::Configuration::Data> &AbstractPipeline::Configuration::StructureLayout::singleUniformStructures() const
{
	return (_singleUniformStructures);
}