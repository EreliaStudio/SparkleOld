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
					enum class Type
					{
						Error,
						Data,
						Indexes,
						ShaderStorage,
						Texture
					};

					struct Attribute
					{
						uint32_t location;
						size_t format;
						size_t offset;
					};

					Type type = Type::Error;
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
				virtual void activate() = 0;
				virtual void deactivate() = 0;

				Description::Buffer description()
				{
					return (_description);
				}

				virtual size_t nbElement()
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
					
					std::string p_inStatement = p_match.str();
					p_result.push_back(p_inStatement);
				}
			}

			return p_result;
		}

		void _parseBuffer(const std::string &p_shaderCode, Storage::Description& p_descriptionToFill)
		{
			std::unordered_set<uint32_t> p_usedLocations;

			p_descriptionToFill.dataBuffer.type = Storage::Description::Buffer::Type::Data;

			p_descriptionToFill.indexesBuffer.type = Storage::Description::Buffer::Type::Indexes;
			p_descriptionToFill.indexesBuffer.stride = sizeof(unsigned int);

			std::unordered_map<std::string, size_t> p_glslTypeToSize = {
				{"float", 1},
				{"vec2", 2},
				{"vec3", 3},
				{"vec4", 4}
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

					if (p_glslTypeToSize.find(p_type) != p_glslTypeToSize.end()) {
						p_attribute.format = p_glslTypeToSize[p_type];
					} else {
						p_attribute.format = 0;
					}

					p_attribute.offset = p_descriptionToFill.dataBuffer.stride;
					p_descriptionToFill.dataBuffer.stride += p_attribute.format * sizeof(float);

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
}