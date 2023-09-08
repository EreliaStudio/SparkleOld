#pragma once

#include <gl/glew.h>
#include <gl/wglew.h>
#include <string>
#include <filesystem>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include "spk_basic_functions.hpp"
#include "math/spk_vector2.hpp"
#include "math/spk_vector3.hpp"
#include "math/spk_matrix4x4.hpp"
#include "miscellaneous/spk_is_vector.hpp"

namespace spk
{
	class AbstractGraphicalDevice
	{
	public:
		class Storage
		{
		public:
			struct Description
			{
				struct Buffer
				{
					enum class Mode
					{
						Error,
						Data,
						Indexes,
						ShaderStorage,
						Texture
					};

					enum class Type
					{
						Error,
						Float,
						UInt,
						Int
					};

					struct Attribute
					{
						Type type = Type::Error;
						uint32_t location = 0;
						size_t format = 1;
						size_t offset = 0;
					};

					Mode mode = Mode::Error;
					size_t stride = 0;
					std::vector<Attribute> attributes;
				};

				Buffer dataBuffer;
				Buffer indexesBuffer;
			};

			class Buffer
			{
			public:

			private:
				Description::Buffer _description;
				size_t _nbElement = 0;

				template <typename... Vectors,
						  typename std::enable_if_t<(... && spk::isVectorType<Vectors>::value), int> = 0>
				void _validateVectorSizes(Vectors &...p_vectors)
				{
					auto firstVectorSize = std::get<0>(std::make_tuple(p_vectors...)).size();

					bool areSameSize = (... && (p_vectors.size() == firstVectorSize));

					if (!areSameSize)
					{
						spk::throwException(L"One or more vectors have different sizes.");
					}
				}

				template <typename T>
				void _addElementToDataBuffer(spk::DataBuffer &p_dataBuffer, size_t p_index, const std::vector<T> &p_vector)
				{
					p_dataBuffer << p_vector[p_index];
				}

				virtual void _pushDataBuffer(const spk::DataBuffer &p_bufferToPush) = 0;

			public:
				Buffer(const Description::Buffer& p_description) :
					_description(p_description)
				{

				}

				virtual void activate() = 0;
				virtual void deactivate() = 0;

				const Description::Buffer& description() const
				{
					return (_description);
				}

				virtual size_t nbElement() const
				{
					return (_nbElement);
				}

				template <typename... Vectors,
						  typename std::enable_if_t<(... && isVectorType<Vectors>::value), int> = 0>
				void push(Vectors &...p_vectors)
				{
					_validateVectorSizes(p_vectors...);

					spk::DataBuffer dataBuffer;
					_nbElement = std::get<0>(std::make_tuple(p_vectors...)).size();

					for (size_t i = 0; i < _nbElement; ++i)
					{
						auto addElementToBuffer = [&](auto &vec)
						{
							_addElementToDataBuffer(dataBuffer, i, vec);
						};

						(addElementToBuffer(p_vectors), ...);
					}

					_pushDataBuffer(dataBuffer);
				}
			};

		private:
			Description _description;
			Buffer *_datas;
			Buffer *_indexes;

			virtual Storage* _duplicate() = 0;
			virtual Buffer* _allocateBuffer(const Description::Buffer& p_bufferDescription) = 0;

		public:
			Storage()
			{
				
			}

			void initialize(const Description &p_description)
			{
				_description = p_description;
				activate();
				_datas = _allocateBuffer(p_description.dataBuffer);
				_indexes = _allocateBuffer(p_description.indexesBuffer);
			}

			virtual Storage *copy()
			{
				Storage *result = _duplicate();
				result->initialize(_description);
				return (result);
			}

			virtual void activate() = 0;
			virtual void deactivate() = 0;

			size_t nbVertex() const
			{
				return (_datas->nbElement());
			}
			size_t nbIndexes() const
			{
				return (_indexes->nbElement());
			}

			Buffer *datas()
			{
				return (_datas);
			}
			Buffer *indexes()
			{
				return (_indexes);
			}
		};

	private:

	protected:
		Storage *_storage;

		std::vector<std::string> _parseBufferInformations(const std::string& p_shaderSource)
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
					
					std::string contact = p_match.str();
					p_result.push_back(contact);
				}
			}

			return p_result;
		}

		void _parseBuffer(const std::string &p_shaderCode, Storage::Description& p_descriptionToFill)
		{
			std::unordered_set<uint32_t> p_usedLocations;

			p_descriptionToFill.dataBuffer.mode = Storage::Description::Buffer::Mode::Data;

			p_descriptionToFill.indexesBuffer.mode = Storage::Description::Buffer::Mode::Indexes;
			p_descriptionToFill.indexesBuffer.stride = 0;

			static const std::unordered_map<std::string, std::tuple<size_t, Storage::Description::Buffer::Type>> glslInputToData = {
				{"float", {1, Storage::Description::Buffer::Type::Float}},
				{"uint", {1, Storage::Description::Buffer::Type::UInt}},
				{"int", {1, Storage::Description::Buffer::Type::Int}},

				{"vec2", {2, Storage::Description::Buffer::Type::Float}},
				{"uvec2", {2, Storage::Description::Buffer::Type::UInt}},
				{"ivec2", {2, Storage::Description::Buffer::Type::Int}},

				{"vec3", {3, Storage::Description::Buffer::Type::Float}},
				{"uvec3", {3, Storage::Description::Buffer::Type::UInt}},
				{"ivec3", {3, Storage::Description::Buffer::Type::Int}},
				
				{"vec4", {4, Storage::Description::Buffer::Type::Float}},
				{"ivec4", {4, Storage::Description::Buffer::Type::Float}},
				{"uvec4", {4, Storage::Description::Buffer::Type::Float}}
			};

			static const std::unordered_map<Storage::Description::Buffer::Type, size_t> typeToSizeMap = {
				{Storage::Description::Buffer::Type::Float, sizeof(float)},
				{Storage::Description::Buffer::Type::UInt, sizeof(unsigned int)},
				{Storage::Description::Buffer::Type::Int, sizeof(int)}
			};

			std::vector<std::string> buffersInformations = _parseBufferInformations(p_shaderCode);

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

					Storage::Description::Buffer::Attribute p_attribute;
					p_attribute.location = p_location;

					auto glslInputToDataIter = glslInputToData.find(p_type);
					if (glslInputToDataIter == glslInputToData.end()) {
						spk::throwException(L"Unsupported data type [" + spk::stringToWString(p_type) + L"]");
					}

					p_attribute.format = std::get<0>(glslInputToDataIter->second);
					p_attribute.type = std::get<1>(glslInputToDataIter->second);

					p_attribute.offset = p_descriptionToFill.dataBuffer.stride;
					p_descriptionToFill.dataBuffer.stride += p_attribute.format * typeToSizeMap.at(p_attribute.type);

					p_descriptionToFill.dataBuffer.attributes.push_back(p_attribute);
				}
			}
		}

		virtual void _loadDevice(
			const std::string &p_vertexModuleName, const std::string &p_vertexModuleCode,
			const std::string &p_fragmentModuleName, const std::string &p_fragmentModuleCode) = 0;

		virtual Storage *_createStorage() = 0;

	public:
		AbstractGraphicalDevice()
		{
			
		}

		void load(const std::wstring &p_vertexShaderCode, const std::wstring &p_fragmentShaderCode)
		{
			_loadDevice("VertexCode", spk::wstringToString(p_vertexShaderCode), "FragmentCode", spk::wstringToString(p_fragmentShaderCode));
		}

		void loadFromFile(const std::filesystem::path &p_vertexShaderPath, const std::filesystem::path &p_fragmentShaderPath)
		{
			std::string vertexShaderContent = getFileContentAsString(p_vertexShaderPath);
			std::string fragmentShaderContent = getFileContentAsString(p_fragmentShaderPath);

			_loadDevice(
					p_vertexShaderPath.filename().string(), vertexShaderContent,
					p_fragmentShaderPath.filename().string(), fragmentShaderContent
				);

			Storage::Description storageDescription;

			_parseBuffer(vertexShaderContent, storageDescription);

			_storage = _createStorage();
			_storage->initialize(storageDescription);
		}

		virtual void activate() = 0;
		virtual void launch(const size_t &p_nbIndexes) = 0;

		Storage *storage()
		{
			return (_storage);
		}
		Storage::Buffer *datas()
		{
			return (_storage->datas());
		}
		Storage::Buffer *indexes()
		{
			return (_storage->indexes());
		}
	};

	std::wstring to_wstring(const spk::AbstractGraphicalDevice::Storage::Description::Buffer::Type& p_input);
}