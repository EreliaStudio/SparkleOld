#include "graphics/pipeline/spk_graphical_api_abstract_pipeline.hpp"
#include <regex>
#include <unordered_map>
#include <unordered_set>

namespace spk::GraphicalAPI
{
	std::vector<std::string> parseBufferCode(const std::string &p_shaderSource)
	{
		std::vector<std::string> result;

		std::stringstream stream(p_shaderSource);
		std::string line;

		std::regex pattern("(layout\\(.*\\)\\s+)?in\\s+\\w+\\s+\\w+");

		while (std::getline(stream, line, ';'))
		{
			std::sregex_iterator it(line.begin(), line.end(), pattern);
			std::sregex_iterator end;

			for (; it != end; ++it)
			{
				std::smatch match = *it;

				std::string contact = match.str();
				result.push_back(contact);
			}
		}

		return result;
	}

	size_t compileLocationAttribute(const std::string& p_locationAttribute, const std::wstring& p_attributeName)
	{
		size_t result;

		if (p_locationAttribute.length() > 0)
		{
			result = std::stoi(p_locationAttribute);
		}
		else
		{
			spk::throwException(L"Buffer [" + p_attributeName + L"] must be explicitly located");
		}

		return (result);
	}

	AbstractPipeline::Object::Storage::Configuration AbstractPipeline::_parseStorageBuffers(const std::string& p_vertexModuleCode)
	{
		AbstractPipeline::Object::Storage::Configuration result;

		result.mode = AbstractPipeline::Object::Storage::Configuration::Mode::Data;

		std::vector<std::string> bufferInfos = parseBufferCode(p_vertexModuleCode);

		static const std::unordered_map<std::string, std::tuple<size_t, AbstractPipeline::Object::Storage::Configuration::Attribute::Type, size_t>> glslInputToData = {
			{"float", {1, AbstractPipeline::Object::Storage::Configuration::Attribute::Type::Float, sizeof(float)}},
			{"uint", {1, AbstractPipeline::Object::Storage::Configuration::Attribute::Type::UInt, sizeof(unsigned int)}},
			{"int", {1, AbstractPipeline::Object::Storage::Configuration::Attribute::Type::Int, sizeof(int)}},

			{"vec2", {2, AbstractPipeline::Object::Storage::Configuration::Attribute::Type::Float, sizeof(float)}},
			{"uvec2", {2, AbstractPipeline::Object::Storage::Configuration::Attribute::Type::UInt, sizeof(unsigned int)}},
			{"ivec2", {2, AbstractPipeline::Object::Storage::Configuration::Attribute::Type::Int, sizeof(int)}},

			{"vec3", {3, AbstractPipeline::Object::Storage::Configuration::Attribute::Type::Float, sizeof(float)}},
			{"uvec3", {3, AbstractPipeline::Object::Storage::Configuration::Attribute::Type::UInt, sizeof(unsigned int)}},
			{"ivec3", {3, AbstractPipeline::Object::Storage::Configuration::Attribute::Type::Int, sizeof(int)}},

			{"vec4", {4, AbstractPipeline::Object::Storage::Configuration::Attribute::Type::Float, sizeof(float)}},
			{"ivec4", {4, AbstractPipeline::Object::Storage::Configuration::Attribute::Type::UInt, sizeof(unsigned int)}},
			{"uvec4", {4, AbstractPipeline::Object::Storage::Configuration::Attribute::Type::Int, sizeof(int)}}
		};

		for (size_t i = 0; i < bufferInfos.size(); i++)
		{
			std::smatch match;
			std::regex p_varPattern(R"((?:layout\s*?\(\s*?location\s*?=\s*?(\d+)\s*?\)\s+)?in\s+(\w+)\s+(\w+))");

			if (std::regex_search(bufferInfos[i], match, p_varPattern))
			{
				AbstractPipeline::Object::Storage::Configuration::Attribute newAttribute;

				auto& glslType = glslInputToData.at(match[2]);

				newAttribute.location = compileLocationAttribute(match[1], spk::to_wstring(match[3]));
				newAttribute.format = std::get<0>(glslType);
				newAttribute.type = std::get<1>(glslType);
				newAttribute.offset = result.stride;
				newAttribute.unitSize = std::get<2>(glslType);

				result.stride += std::get<2>(glslType) * newAttribute.format;
				result.attributes[spk::to_wstring(match[3])] = newAttribute;
			}
		}

		result.inverseOffset();

		return (result);
	}
}