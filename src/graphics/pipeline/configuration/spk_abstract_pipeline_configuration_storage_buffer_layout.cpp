#include "graphics/pipeline/spk_abstract_pipeline.hpp"

#include <regex>

#include "spk_basic_functions.hpp"

namespace spk
{
    AbstractPipeline::Configuration::StorageBufferLayout::StorageBufferLayout(const StructureLayout& p_structureLayout) :
        FieldArrayLayout(p_structureLayout)
    {
    }

    void AbstractPipeline::Configuration::StorageBufferLayout::treat(const ShaderModule::Instruction &p_instruction)
    {
        std::regex re(R"(layout\(location=(\d+)\)\s+in\s+(\w+)\s+(\w+);)");
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
                spk::throwException(L"Unexpected StorageBufferLayout type [" + spk::to_wstring(dataType) + L"] inside [" + spk::to_wstring(p_instruction.code) + L"]");
            }
        }
        else
        {
            spk::throwException(L"Unexpected StorageBufferLayout instruction [" + spk::to_wstring(p_instruction.code) + L"]\nExpected format :\nlayout(location=[LOCATION_ID]) in [TYPE] [NAME]");
        }
    }
}