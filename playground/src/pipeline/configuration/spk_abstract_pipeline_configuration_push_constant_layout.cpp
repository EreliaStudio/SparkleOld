#include "pipeline/pipeline.hpp"

#include <regex>

AbstractPipeline::Configuration::PushConstantLayout::PushConstantLayout(const StructureLayout& p_structureLayout) :
	FieldArrayLayout(p_structureLayout)
{
}

void AbstractPipeline::Configuration::PushConstantLayout::treat(const ShaderModule::Instruction &p_instruction)
{
    std::regex outer_re(R"(layout\(push_constant\) uniform (.*?) \{(.*?)\} (.*?);)");
    std::smatch outer_match;

    if (std::regex_search(p_instruction.code, outer_match, outer_re))
    {
        _type = spk::to_wstring(outer_match[1]);
        std::string innerCode = outer_match[2];
        _name = spk::to_wstring(outer_match[3]);

        std::regex inner_re(R"((\w+)\s+(\w+)\s*;)");
        std::smatch inner_match;

        std::string::const_iterator searchStart(innerCode.cbegin());
        while (std::regex_search(searchStart, innerCode.cend(), inner_match, inner_re))
        {
            std::string dataType = inner_match[1];
            std::string variableName = inner_match[2];

            auto it = structureLayout.structures().find(dataType);
            if (it != structureLayout.structures().end())
            {
                Data pushConstantData = it->second;
                insert(variableName, pushConstantData, 0);
            }
            else
            {
                spk::throwException(L"Unexpected PushConstant type [" + spk::to_wstring(dataType) + L"] inside [" + spk::to_wstring(p_instruction.code) + L"]");
            }

            searchStart = inner_match.suffix().first;
        }
    }
    else
    {
        spk::throwException(L"Unexpected PushConstant instruction [" + spk::to_wstring(p_instruction.code) + L"]\nExpected format :\nlayout(push_constant) uniform [PushConstantStructure] \n{\n    [ValueType] [ValueName];\n    [Repeat for each attribute];\n} [PushConstantName];");
    }
}