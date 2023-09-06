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

				struct Uniform
				{	
					enum Type
					{
						Error,
						Float,
						Int,
						UInt,
						Vec2,
						Vec2Int,
						Vec2UInt,
						Vec3,
						Vec3Int,
						Vec3UInt,
						Mat4x4,
						FloatArray,
						IntArray,
						UIntArray
					};

					std::wstring name = L"Unnamed";
					Type type = Type::Error;
				};

				Buffer dataBuffer;
				Buffer indexesBuffer;
				std::vector<Uniform> uniforms;
			};

			class Buffer
			{
			public:

			private:
				Description::Buffer _description;

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

				virtual size_t nbElement() = 0;

				template <typename... Vectors,
						  typename std::enable_if_t<(... && isVectorType<Vectors>::value), int> = 0>
				void push(Vectors &...p_vectors)
				{
					_validateVectorSizes(p_vectors...);

					spk::DataBuffer dataBuffer;
					const auto size = std::get<0>(std::make_tuple(p_vectors...)).size();

					for (size_t i = 0; i < size; ++i)
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

			class Uniform
			{
			public:
			private:
				Description::Uniform _description;

				void _checkTypeValidity(const Description::Uniform::Type &p_pushingType)
				{
					if (_description.type != p_pushingType)
						spk::throwException(L"Error while pushing data to uniform [" + _description.name + L"] - Invalid type");
				}

				virtual void _push(float p_value) = 0;
				virtual void _push(int p_value) = 0;
				virtual void _push(unsigned int p_value) = 0;

				virtual void _push(const spk::Vector2 &p_value) = 0;
				virtual void _push(const spk::Vector2Int &p_value) = 0;
				virtual void _push(const spk::Vector2UInt &p_value) = 0;

				virtual void _push(const spk::Vector3 &p_value) = 0;
				virtual void _push(const spk::Vector3Int &p_value) = 0;
				virtual void _push(const spk::Vector3UInt &p_value) = 0;

				virtual void _push(const spk::Matrix4x4 &p_value) = 0;

				virtual void _push(const std::vector<float> &p_values) = 0;
				virtual void _push(const std::vector<int> &p_values) = 0;
				virtual void _push(const std::vector<unsigned int> &p_values) = 0;
			public:
				Uniform(const Description::Uniform& p_description) :
					_description(p_description)
				{

				}

				const std::wstring& name() const
				{
					return (_description.name);
				}

				void push(float p_value)
				{
#ifndef NDEBUG
					_checkTypeValidity(Description::Uniform::Type::Float);
#endif
					_push(p_value);
				}
				void push(int p_value)
				{
#ifndef NDEBUG
					_checkTypeValidity(Description::Uniform::Type::Int);
#endif
					_push(p_value);
				}
				void push(unsigned int p_value)
				{
#ifndef NDEBUG
					_checkTypeValidity(Description::Uniform::Type::UInt);
#endif
					_push(p_value);
				}
				void push(const Vector2 &p_value)
				{
#ifndef NDEBUG
					_checkTypeValidity(Description::Uniform::Type::Vec2);
#endif
					_push(p_value);
				}
				void push(const Vector2Int &p_value)
				{
#ifndef NDEBUG
					_checkTypeValidity(Description::Uniform::Type::Vec2Int);
#endif
					_push(p_value);
				}
				void push(const Vector2UInt &p_value)
				{
#ifndef NDEBUG
					_checkTypeValidity(Description::Uniform::Type::Vec2UInt);
#endif
					_push(p_value);
				}
				void push(const Vector3 &p_value)
				{
#ifndef NDEBUG
					_checkTypeValidity(Description::Uniform::Type::Vec3);
#endif
					_push(p_value);
				}
				void push(const Vector3Int &p_value)
				{
#ifndef NDEBUG
					_checkTypeValidity(Description::Uniform::Type::Vec3Int);
#endif
					_push(p_value);
				}
				void push(const Vector3UInt &p_value)
				{
#ifndef NDEBUG
					_checkTypeValidity(Description::Uniform::Type::Vec3UInt);
#endif
					_push(p_value);
				}
				void push(const Matrix4x4 &p_value)
				{
#ifndef NDEBUG
					_checkTypeValidity(Description::Uniform::Type::Mat4x4);
#endif
					_push(p_value);
				}
				void push(const std::vector<float> &p_values)
				{
#ifndef NDEBUG
					_checkTypeValidity(Description::Uniform::Type::FloatArray);
#endif
					_push(p_values);
				}
				void push(const std::vector<int> &p_values)
				{
#ifndef NDEBUG
					_checkTypeValidity(Description::Uniform::Type::IntArray);
#endif
					_push(p_values);
				}
				void push(const std::vector<unsigned int> &p_values)
				{
#ifndef NDEBUG
					_checkTypeValidity(Description::Uniform::Type::UIntArray);
#endif
					_push(p_values);
				}
			};

		private:
			Description _description;
			Buffer *_datas;
			Buffer *_indexes;
			std::map<std::wstring, Uniform *> _uniforms;

			virtual Storage* _allocateStorage() = 0;
			virtual Buffer* _allocateBuffer(const Description::Buffer& p_bufferDescription) = 0;
			virtual Uniform* _allocateUniform(const Description::Uniform& p_uniformDescription) = 0;

		public:
			Storage()
			{
				
			}

			void initialize(const Description &p_description)
			{
				_description = p_description;
				_datas = _allocateBuffer(p_description.dataBuffer);
				_indexes = _allocateBuffer(p_description.indexesBuffer);
				for (size_t i = 0; i < p_description.uniforms.size(); i++)
				{
					Uniform* newUniform = _allocateUniform(p_description.uniforms[i]);

					_uniforms[newUniform->name()] = newUniform;
				}
			}

			virtual Storage *copy()
			{
				Storage *result = _allocateStorage();
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
			std::map<std::wstring, Uniform *> &uniforms()
			{
				return (_uniforms);
			}
			Uniform *uniform(const std::wstring &p_uniformName)
			{
				if (_uniforms.contains(p_uniformName) == false)
					spk::throwException(L"Uniform [" + p_uniformName + L"] does not exist");
				return (_uniforms[p_uniformName]);
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
		void _parseUniform(const std::string &p_shaderCode, Storage::Description& p_descriptionToFill)
		{
			
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

			_parseUniform(vertexShaderContent, storageDescription);
			_parseUniform(fragmentShaderContent, storageDescription);

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
		std::map<std::wstring, Storage::Uniform *> &uniforms()
		{
			return (_storage->uniforms());
		}
		Storage::Uniform *uniform(const std::wstring &p_uniformName)
		{
			return (_storage->uniform(p_uniformName));
		}
	};
}