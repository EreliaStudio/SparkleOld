#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"
#include <regex>

namespace spk::GraphicalAPI
{

	void ShaderLayout::parseStructInstruction(const std::string &p_instruction)
	{
		std::string input = "struct Test{vec2 position;vec4 color;}";
		std::regex re(R"(struct\s+(\w+)\s*\{([^\}]+)\})");
		std::smatch match;

		if (std::regex_search(input, match, re)) {
			std::string structName = match[1].str();
			std::string innerData = match[2].str();

			size_t structureSize = 0;

			std::regex re2(R"((\w+)\s+(\w+)\s*;)");
			std::string::const_iterator searchStart(innerData.cbegin());
			while (std::regex_search(searchStart, innerData.cend(), match, re2)) {
				std::string type = match[1];
				std::string varName = match[2];

				if (dataTypes.contains(type) == false)
				{
					spk::throwException(L"Structure [" + spk::to_wstring(type) + L"] not recognized");
				}

				auto& data = dataTypes.at(type);

				structureSize += data.size * data.format;

				searchStart = match.suffix().first;
			}

			dataTypes[structName] = Data(structureSize);
		}
	}
}