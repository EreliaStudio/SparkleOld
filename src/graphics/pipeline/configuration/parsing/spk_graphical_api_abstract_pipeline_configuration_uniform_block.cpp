#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"
#include <regex>

namespace spk::GraphicalAPI
{
	std::string trimWhitespace(const std::string &str);
	std::string removeComments(const std::string& line);
	bool splitDeclaration(const std::string &line, std::string &outDataType, std::string &outVarName);

AbstractPipeline::Configuration::UniformBlockLayout::Block parseShaderUniformBody(
    AbstractPipeline::Configuration& p_configuration, 
    const std::string& p_contents, 
    unsigned int p_set, 
    unsigned int p_binding)
	{
		std::stringstream ss(p_contents);
		std::string line;
		AbstractPipeline::Configuration::UniformBlockLayout::Block newBlock;
		newBlock.stride = 0;

		while (std::getline(ss, line))
		{
			line = trimWhitespace(removeComments(line));
			if (line.empty())
				continue;

			std::string dataType, varName;
			if (splitDeclaration(line, dataType, varName) == false)
				spk::throwException(L"Invalid uniform block definition: [" + spk::to_wstring(line) + L"]");

			AbstractPipeline::Configuration::UniformBlockLayout::Block::Field field;
			auto it = p_configuration.dataTypes.find(dataType);
			if (it == p_configuration.dataTypes.end())
				spk::throwException(L"Data type [" + spk::to_wstring(dataType) + L"] not found");

			field.attribute = it->second;
			field.offset = newBlock.stride;
			field.name = spk::to_wstring(varName);
			newBlock.fields.push_back(field);
			newBlock.stride += field.attribute.format * field.attribute.size;
		}

		return newBlock;
	}

	void parseShaderUniforms(AbstractPipeline::Configuration& p_configuration, const std::string& p_shaderCode)
	{
		std::regex uniform_block_pattern(R"(layout\s*\(set\s*=\s*(\d+),\s*binding\s*=\s*(\d+)\)\s*uniform\s+(\w+)\s*\{\s*([^\}]*)\s*\}\s*(\w+)\s*;)");

		std::sregex_iterator iter(p_shaderCode.begin(), p_shaderCode.end(), uniform_block_pattern);
		std::sregex_iterator end;

		while (iter != end)
		{
			std::smatch match = *iter;

			unsigned int set = std::stoi(match[1]);
			unsigned int binding = std::stoi(match[2]);
			std::string blockName = match[3];
			std::string contents = match[4];
			std::string instanceName = match[5];

			AbstractPipeline::Configuration::UniformBlockLayout::Block newBlock = parseShaderUniformBody(p_configuration, contents, set, binding);

			AbstractPipeline::Configuration::UniformBlockLayout::Block::Key key { set, binding };

			p_configuration.uniformBlocks.uniforms[key] = newBlock;
			p_configuration.uniformBlocks.uniformKeys[spk::to_wstring(instanceName)] = key;

			++iter;
		}

		for (auto& blockKey : p_configuration.uniformBlocks.uniformKeys)
		{
			spk::cout << "Block [" << blockKey.first << "] - Set [" << blockKey.second.set << "] / binding [" << blockKey.second.binding << "]" << std::endl;
			spk::cout << p_configuration.uniformBlocks.uniforms[blockKey.second] << std::endl;
		}
	}
}