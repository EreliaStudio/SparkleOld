#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"
#include <regex>
#include <unordered_map>
#include <unordered_set>

#include <csignal>

namespace spk::GraphicalAPI
{
	std::vector<std::string> parseBufferCode(const std::string &p_shaderSource)
	{
		std::vector<std::string> p_result;

		std::stringstream p_stream(p_shaderSource);
		std::string p_line;

		std::regex p_pattern("(layout\\(.*\\)\\s+)?in\\s+\\w+\\s+\\w+");

		while (std::getline(p_stream, p_line, ';'))
		{
			std::sregex_iterator p_it(p_line.begin(), p_line.end(), p_pattern);
			std::sregex_iterator p_end;

			for (; p_it != p_end; ++p_it)
			{
				std::smatch match = *p_it;

				std::string contact = match.str();
				p_result.push_back(contact);
			}
		}

		return p_result;
	}

	std::unordered_set<uint32_t> usedLocations;
	uint32_t nextAvailableLocation = 0;

	size_t compileLocationValue(const std::string& p_locationValue)
	{
		size_t result;

		if (p_locationValue.length() > 0)
		{
			result = std::stoi(p_locationValue);
			usedLocations.insert(result);
		}
		else
		{
			while (usedLocations.find(nextAvailableLocation) != usedLocations.end())
			{
				++nextAvailableLocation;
			}
			result = nextAvailableLocation;
			usedLocations.insert(nextAvailableLocation);
		}

		return (result);
	}

	AbstractPipeline::Object::Storage::Configuration AbstractPipeline::_parseStorageBuffers(const std::string& p_vertexModuleCode)
	{
		usedLocations.clear();
		nextAvailableLocation = 0;

		AbstractPipeline::Object::Storage::Configuration result;

		std::vector<std::string> bufferInfos = parseBufferCode(p_vertexModuleCode);

		static const std::unordered_map<std::string, std::tuple<size_t, AbstractPipeline::Object::Storage::Configuration::Value::Type, size_t>> glslInputToData = {
			{"float", {1, AbstractPipeline::Object::Storage::Configuration::Value::Type::Float, sizeof(float)}},
			{"uint", {1, AbstractPipeline::Object::Storage::Configuration::Value::Type::UInt, sizeof(unsigned int)}},
			{"int", {1, AbstractPipeline::Object::Storage::Configuration::Value::Type::Int, sizeof(int)}},

			{"vec2", {2, AbstractPipeline::Object::Storage::Configuration::Value::Type::Float, sizeof(float)}},
			{"uvec2", {2, AbstractPipeline::Object::Storage::Configuration::Value::Type::UInt, sizeof(unsigned int)}},
			{"ivec2", {2, AbstractPipeline::Object::Storage::Configuration::Value::Type::Int, sizeof(int)}},

			{"vec3", {3, AbstractPipeline::Object::Storage::Configuration::Value::Type::Float, sizeof(float)}},
			{"uvec3", {3, AbstractPipeline::Object::Storage::Configuration::Value::Type::UInt, sizeof(unsigned int)}},
			{"ivec3", {3, AbstractPipeline::Object::Storage::Configuration::Value::Type::Int, sizeof(int)}},

			{"vec4", {4, AbstractPipeline::Object::Storage::Configuration::Value::Type::Float, sizeof(float)}},
			{"ivec4", {4, AbstractPipeline::Object::Storage::Configuration::Value::Type::UInt, sizeof(unsigned int)}},
			{"uvec4", {4, AbstractPipeline::Object::Storage::Configuration::Value::Type::Int, sizeof(int)}}
		};

		for (size_t i = 0; i < bufferInfos.size(); i++)
		{
			std::smatch match;
			std::regex p_varPattern(R"((?:layout\s*?\(\s*?location\s*?=\s*?(\d+)\s*?\)\s+)?in\s+(\w+)\s+(\w+))");

			if (std::regex_search(bufferInfos[i], match, p_varPattern))
			{
				AbstractPipeline::Object::Storage::Configuration::Value newValue;

				newValue.location = compileLocationValue(match[1]);
				newValue.offset = result.totalSize;

				auto& glslType = glslInputToData.at(match[2]);
				newValue.size = std::get<0>(glslType);
				newValue.type = std::get<1>(glslType);
				result.totalSize += std::get<2>(glslType) * newValue.size;

				result.values[spk::to_wstring(match[3])] = newValue;
			}
		}

		return (result);
	}
}