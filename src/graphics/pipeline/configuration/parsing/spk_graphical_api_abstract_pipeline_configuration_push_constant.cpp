#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

namespace spk::GraphicalAPI
{
	std::string trimWhitespace(const std::string &str)
	{
		auto start = std::find_if_not(str.begin(), str.end(), ::isspace);
		auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();
		return (start < end) ? std::string(start, end) : std::string();
	}

	std::string removeComments(const std::string& line)
	{
		size_t commentIndex = line.find("//");
		return (commentIndex != std::string::npos) ? line.substr(0, commentIndex) : line;
	}

	bool splitDeclaration(const std::string &line, std::string &outDataType, std::string &outVarName)
	{
		size_t spaceIndex = line.find(' ');
		if (spaceIndex == std::string::npos)
		{
			return false;
		}
		outDataType = trimWhitespace(line.substr(0, spaceIndex));
		outVarName = trimWhitespace(line.substr(spaceIndex + 1));
		return true;
	}

	void parseShaderPushConstantsBody(AbstractPipeline::Configuration &p_configuration, const std::string &p_pushConstantBody)
	{
		std::stringstream ss(p_pushConstantBody);
		std::string line;

		while (std::getline(ss, line))
		{
			line = trimWhitespace(removeComments(line));
			if (line.empty())
				continue;

			std::string dataType, varName;
			if (splitDeclaration(line, dataType, varName) == false)
				spk::throwException(L"Invalid push_constant definition : [" + spk::to_wstring(line) + L"]");

			AbstractPipeline::Configuration::PushConstantLayout::Field field;
			auto it = p_configuration.dataTypes.find(dataType);
			if (it == p_configuration.dataTypes.end())
				spk::throwException(L"Structure [" + spk::to_wstring(dataType) + L"] not found");

			field.attribute = it->second;
			field.offset = p_configuration.constants.stride;
			p_configuration.constants.fields.push_back(field);
			p_configuration.constants.stride += field.attribute.format * field.attribute.size;
		}
	}

	void parseShaderPushConstants(AbstractPipeline::Configuration &p_configuration, const std::string &p_shaderCode)
	{
		std::regex push_constant_pattern(R"(layout\s*\(\s*push_constant\s*\)\s*uniform\s+(\w+)\s*\{\s*([^\}]*)\s*\}\s*(\w+)\s*;)");

		std::smatch match;
		if (std::regex_search(p_shaderCode, match, push_constant_pattern))
		{
			std::string blockName = match[1];
			std::string contents = match[2];
			std::string instanceName = match[3];

			parseShaderPushConstantsBody(p_configuration, contents);
		}
	}
}