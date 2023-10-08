#include "pipeline/spk_abstract_pipeline.hpp"

#include <regex>

AbstractPipeline::Configuration::OutputBufferLayout::OutputBufferLayout(const StructureLayout &p_structureLayout) : FieldArrayLayout(p_structureLayout)
{
}

void AbstractPipeline::Configuration::OutputBufferLayout::treat(const ShaderModule::Instruction &p_instruction)
{
    std::regex re(R"(layout\(location=(\d+)\)\s+out\s+(\w+)\s+(\w+);)");
    std::smatch match;

    if (std::regex_search(p_instruction.code, match, re))
    {
        size_t location = std::stoi(match[1].str());
        std::string dataType = match[2].str();
        std::string attributeName = match[3];

        auto it = structureLayout.structures().find(dataType);
        if (it != structureLayout.structures().end())
        {
            Data outputData = it->second;

            this->insert(attributeName, outputData, location);
        }
        else
        {
            spk::throwException(L"Unexpected OutputLayout type [" + spk::to_wstring(dataType) + L"] inside [" + spk::to_wstring(p_instruction.code) + L"]");
        }
    }
    else
    {
        spk::throwException(L"Unexpected OutputLayout instruction [" + spk::to_wstring(p_instruction.code) + L"]\nExpected format :\nlayout(location=[LOCATION_ID]) out [TYPE] [NAME]");
    }
}