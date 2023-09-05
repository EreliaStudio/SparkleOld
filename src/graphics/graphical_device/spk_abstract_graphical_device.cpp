#include "graphics/graphical_device/spk_abstract_graphical_device.hpp"
#include <regex>
#include <unordered_map>
#include <unordered_set>

namespace spk
{
	std::vector<std::string> parseBufferInformations(const std::string& p_shaderSource)
	{
		std::vector<std::string> p_result;

		std::stringstream p_stream(p_shaderSource);
		std::string p_line;

		std::regex p_pattern("(layout\\(.*\\)\\s+)?in\\s+\\w+\\s+\\w+");

		while (std::getline(p_stream, p_line, ';')) {
			std::sregex_iterator p_it(p_line.begin(), p_line.end(), p_pattern);
			std::sregex_iterator p_end;
			
			for (; p_it != p_end; ++p_it) {
				std::smatch p_match = *p_it;
				
				std::string p_inStatement = p_match.str();
				p_result.push_back(p_inStatement);
			}
		}

		return p_result;
	}

	void AbstractGraphicalDevice::_parseBuffer(const std::string& p_shaderCode)
	{
		Storage::Element dataBufferElement;
		Storage::Element elementBufferElement;
		size_t p_currentOffset = 0;
		std::unordered_set<uint32_t> p_usedLocations;

		dataBufferElement.type = Storage::Element::Type::Data;
		elementBufferElement.type = Storage::Element::Type::Indexes;
		elementBufferElement.stride = sizeof(unsigned int);

		std::unordered_map<std::string, size_t> p_glslTypeToSize = {
			{"float", 1},
			{"vec2", 2},
			{"vec3", 3},
			{"vec4", 4}
		};

		std::vector<std::string> buffersInformations = parseBufferInformations(p_shaderCode);

		uint32_t p_nextAvailableLocation = 0;

		for (size_t i = 0; i < buffersInformations.size(); i++)
		{
			std::smatch p_match;
			std::regex p_varPattern(R"((?:layout\s*?\(\s*?location\s*?=\s*?(\d+)\s*?\)\s+)?in\s+(\w+)\s+(\w+))");

			if (std::regex_search(buffersInformations[i], p_match, p_varPattern))
			{
				uint32_t p_location;
				if (p_match[1].length() > 0) {
					p_location = std::stoi(p_match[1]);
					p_usedLocations.insert(p_location);
				} else {
					while (p_usedLocations.find(p_nextAvailableLocation) != p_usedLocations.end()) {
						++p_nextAvailableLocation;
					}
					p_location = p_nextAvailableLocation;
					p_usedLocations.insert(p_nextAvailableLocation);
				}

				std::string p_type = p_match[2];
				std::string p_name = p_match[3];

				Storage::Element::Attribute p_attribute;
				p_attribute.location = p_location;

				if (p_glslTypeToSize.find(p_type) != p_glslTypeToSize.end()) {
					p_attribute.format = p_glslTypeToSize[p_type];
				} else {
					p_attribute.format = 0;
				}

				p_attribute.offset = p_currentOffset;
				p_currentOffset += p_attribute.format;

				dataBufferElement.attributes.push_back(p_attribute);
				dataBufferElement.stride += p_attribute.format * sizeof(float);
			}
		}
		if (dataBufferElement.attributes.size() == 0)
			dataBufferElement.stride = 0;

		_dataStorage = _createStorage(dataBufferElement);
		_indexesStorage = _createStorage(elementBufferElement);
	}

	std::vector<std::string> extractUniformsInformations(const std::string& p_shaderSource)
	{
		std::vector<std::string> p_result;

		std::stringstream p_stream(p_shaderSource);
		std::string p_statement;

		std::regex p_pattern("uniform\\s+\\w+\\s+\\w+");

		while (std::getline(p_stream, p_statement, ';')) {
			std::sregex_iterator p_it(p_statement.begin(), p_statement.end(), p_pattern);
			std::sregex_iterator p_end;
			
			for (; p_it != p_end; ++p_it) {
				std::smatch p_match = *p_it;
				
				std::string p_uniformStatement = p_match.str();
				p_result.push_back(p_uniformStatement);
			}
		}

		return p_result;
	}

	void AbstractGraphicalDevice::_parseUniform(const std::string& p_shaderCode)
	{
		
	}

	AbstractGraphicalDevice::AbstractGraphicalDevice()
	{
		
	}

	void AbstractGraphicalDevice::load(const std::wstring& p_vertexShaderCode, const std::wstring& p_fragmentShaderCode)
	{
		_loadDevice("VertexCode", spk::wstringToString(p_vertexShaderCode), "FragmentCode", spk::wstringToString(p_fragmentShaderCode));
	}

	void AbstractGraphicalDevice::loadFromFile(const std::filesystem::path& p_vertexShaderPath, const std::filesystem::path& p_fragmentShaderPath)
	{
		std::string vertexShaderContent = getFileContentAsString(p_vertexShaderPath);
		std::string fragmentShaderContent = getFileContentAsString(p_fragmentShaderPath);

		_loadDevice(
				p_vertexShaderPath.filename().string(), vertexShaderContent,
				p_fragmentShaderPath.filename().string(), fragmentShaderContent
			);

		_parseBuffer(vertexShaderContent);

		_parseUniform(vertexShaderContent);
		_parseUniform(fragmentShaderContent);
	}

	AbstractGraphicalDevice::Storage *AbstractGraphicalDevice::dataStorage()
	{
		return (_dataStorage);
	}

	AbstractGraphicalDevice::Storage *AbstractGraphicalDevice::indexesStorage()
	{
		return (_indexesStorage);
	}
	
	std::map<std::wstring, AbstractGraphicalDevice::AbstractUniform *> &AbstractGraphicalDevice::uniforms()
	{
		return (_uniforms);
	}
	
	AbstractGraphicalDevice::AbstractUniform *AbstractGraphicalDevice::uniform(const std::wstring &p_uniformName)
	{
		if (_uniforms.contains(p_uniformName) == false)
		{
			spk::throwException(L"Can't find uniform named [" + p_uniformName + L"]");
		}
		return (_uniforms[p_uniformName]);
	}
}