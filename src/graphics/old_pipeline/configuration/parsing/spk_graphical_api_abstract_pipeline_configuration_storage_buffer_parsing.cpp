#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"
#include <regex>

namespace spk::GraphicalAPI
{
	void AbstractPipeline::Configuration::parseLayoutBufferInstruction(const std::string &p_instruction)
	{
		std::regex bufferRegex("layout\\s*(?:\\(location\\s*=\\s*(\\d+)\\))?\\s*in\\s*(\\w+)\\s*(\\w+)");
		std::smatch bufferMatch;

		if (std::regex_search(p_instruction, bufferMatch, bufferRegex))
		{
			std::string locationStr = bufferMatch[1].str();
			std::string dataTypeStr = bufferMatch[2].str();
			std::string variableName = bufferMatch[3].str();

			if (locationStr.empty())
			{
				spk::throwException(L"Location is not set for: " + spk::to_wstring(variableName));
			}
			else
			{
				size_t location = static_cast<size_t>(std::stoi(locationStr));

				if (dataTypes.find(dataTypeStr) != dataTypes.end())
				{
					Data fieldData = dataTypes[dataTypeStr];

					StorageLayout::Field newField;
					newField.location = location;
					newField.attribute = fieldData;
					newField.offset = storage.stride;

					storage.fields.push_back(newField);

					storage.stride += fieldData.format * fieldData.size;
				}
				else
				{
					spk::throwException(L"Unrecognized data type: " + spk::to_wstring(dataTypeStr));
				}
			}
		}
		else
		{
			spk::throwException(L"Invalid buffer instruction: " + spk::to_wstring(p_instruction));
		}
	}
}