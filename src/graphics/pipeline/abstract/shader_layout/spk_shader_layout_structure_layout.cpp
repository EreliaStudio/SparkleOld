#include "graphics/pipeline/spk_shader_layout.hpp"

#include <regex>

#include "spk_basic_functions.hpp"

namespace spk
{
    ShaderLayout::StructureLayout::StructureLayout()
    {
        _SamplerUniformStructures = {
            {"sampler1D", Data(Data::Type::Int, 1, sizeof(int), 1)},
            {"sampler2D", Data(Data::Type::Int, 1, sizeof(int), 1)},
            {"sampler3D", Data(Data::Type::Int, 1, sizeof(int), 1)},
            {"samplerCube", Data(Data::Type::Int, 1, sizeof(int), 1)},
        };

        _acceptedSamplerUniformTypeString = L"sampler1D, sampler2D, sampler3D, samplerCube";
    }

    void ShaderLayout::StructureLayout::reset()
    {
        _structures = {
            {"float", Data(Data::Type::Float, 1,   sizeof(float),        4)},
            {"int",   Data(Data::Type::Int,   1,   sizeof(int),          4)}, 
            {"uint",  Data(Data::Type::UInt,  1,   sizeof(unsigned int), 4)},

            {"vec2",  Data(Data::Type::Float, 2,   sizeof(float),        8)},
            {"ivec2", Data(Data::Type::Int,   2,   sizeof(int),          8)},
            {"uvec2", Data(Data::Type::UInt,  2,   sizeof(unsigned int), 8)},

            {"vec3",  Data(Data::Type::Float, 3,   sizeof(float),        16)},
            {"ivec3", Data(Data::Type::Int,   3,   sizeof(int),          16)},
            {"uvec3", Data(Data::Type::UInt,  3,   sizeof(unsigned int), 16)},

            {"vec4",  Data(Data::Type::Float, 4,   sizeof(float),        16)},
            {"ivec4", Data(Data::Type::Int,   4,   sizeof(int),          16)},
            {"uvec4", Data(Data::Type::UInt,  4,   sizeof(unsigned int), 16)},

            {"mat3",  Data(Data::Type::Float, 9,   sizeof(float),        16)},
            {"mat4",  Data(Data::Type::Float, 16,  sizeof(float),        16)},
        };
    }


    void ShaderLayout::StructureLayout::treat(const ShaderModule::Instruction &p_instruction)
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

            size_t currentOffset = 0;
            size_t totalStride = 0;

            while (std::regex_search(searchStart, fields.cend(), field_match, field_re))
            {
                std::string dataType = field_match[1];
                
                auto it = _structures.find(dataType);
                if (it != _structures.end())
                {
                    if ((currentOffset % it->second.paddingFormat) != 0)
                        currentOffset += (it->second.paddingFormat - (currentOffset % it->second.paddingFormat));

                    currentOffset += it->second.format * it->second.size;
                    
                    while (currentOffset > totalStride)
                        totalStride += 4;
                }
                else
                {
                    spk::throwException(L"Unexpected Structure member type [" + spk::to_wstring(dataType) + L"] inside [" + spk::to_wstring(p_instruction.code) + L"]");
                }

                searchStart = field_match.suffix().first;
            }

            Data structData(Data::Type::Structure, 1, totalStride, 4);
            
            _structures[structName] = structData;
        }
        else
        {
            spk::throwException(L"Unexpected Structure instruction [" + spk::to_wstring(p_instruction.code) + L"]\nExpected format :\nstruct [STRUCTURE_NAME]\n{\n    [ValueType] [ValueName];\n    [Repeat for each attribute];\n};");
        }
    }

    std::wostream& operator<<(std::wostream& p_out, const ShaderLayout::StructureLayout& p_layout)
    {
        p_out << "\t\tStructures: " << std::endl;
        for (const auto& pair : p_layout.structures())
        {
            p_out << "\t\t\t" << spk::to_wstring(pair.first) << ": " << pair.second << std::endl;
        }
        
        p_out << "\t\tSamplerUniformStructures: " << std::endl;
        for (const auto& pair : p_layout.SamplerUniformStructures())
        {
            p_out << "\t\t\t" << spk::to_wstring(pair.first) << ": " << pair.second << std::endl;
        }
        return p_out;
    }

    const std::wstring& ShaderLayout::StructureLayout::acceptedSamplerUniformTypeString() const
    {
        return (_acceptedSamplerUniformTypeString);
    }

    const std::map<std::string, ShaderLayout::Data> &ShaderLayout::StructureLayout::structures() const
    {
        return (_structures);
    }

    const std::map<std::string, ShaderLayout::Data> &ShaderLayout::StructureLayout::SamplerUniformStructures() const
    {
        return (_SamplerUniformStructures);
    }
}