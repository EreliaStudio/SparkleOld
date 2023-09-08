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
#include "graphics/spk_color.hpp"
#include "miscellaneous/spk_is_vector.hpp"

#include "graphics/spk_window.hpp"

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

				struct Descriptor
				{
					struct Uniform
					{
						enum class Type
						{
							Error,
							Float,
							Int,
							UInt,
							Vector2,
							Vector2Int,
							Vector2UInt,
							Vector3,
							Vector3Int,
							Vector3UInt,
							Vector4,
							Matrix4x4
						};

						Type type = Type::Error;
						std::wstring name = L"Unnamed";
						size_t size = sizeof(float);
						size_t offset = 0;
					};

					std::vector<Uniform> uniforms;
					size_t uniformSize = 0;
				};

				Buffer dataBuffer;
				Buffer indexesBuffer;
				Descriptor descriptor;
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
				Buffer(const Description::Buffer &p_description) : _description(p_description)
				{
				}

				virtual void activate() = 0;
				virtual void deactivate() = 0;

				const Description::Buffer &description() const
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

			class Descriptor
			{
				friend class Storage;

			public:
				class Uniform
				{
				private:
					Description::Descriptor::Uniform _description;

					void _checkTypeValidity(const Description::Descriptor::Uniform::Type &p_pushingType)
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

					virtual void _push(const spk::Color &p_value) = 0;

					virtual void _push(const spk::Matrix4x4 &p_value) = 0;

				public:
					Uniform(const Description::Descriptor::Uniform &p_description) : _description(p_description)
					{
					}

					void push(float p_value)
					{
#ifndef NDEBUG
						_checkTypeValidity(Description::Descriptor::Uniform::Type::Float);
#endif
						_push(p_value);
					}
					void push(int p_value)
					{
#ifndef NDEBUG
						_checkTypeValidity(Description::Descriptor::Uniform::Type::Int);
#endif
						_push(p_value);
					}

					void push(unsigned int p_value)
					{
#ifndef NDEBUG
						_checkTypeValidity(Description::Descriptor::Uniform::Type::UInt);
#endif
						_push(p_value);
					}

					void push(const Vector2 &p_value)
					{
#ifndef NDEBUG
						_checkTypeValidity(Description::Descriptor::Uniform::Type::Vector2);
#endif
						_push(p_value);
					}

					void push(const Vector2Int &p_value)
					{
#ifndef NDEBUG
						_checkTypeValidity(Description::Descriptor::Uniform::Type::Vector2Int);
#endif
						_push(p_value);
					}

					void push(const Vector2UInt &p_value)
					{
#ifndef NDEBUG
						_checkTypeValidity(Description::Descriptor::Uniform::Type::Vector2UInt);
#endif
						_push(p_value);
					}

					void push(const Vector3 &p_value)
					{
#ifndef NDEBUG
						_checkTypeValidity(Description::Descriptor::Uniform::Type::Vector3);
#endif
						_push(p_value);
					}

					void push(const Vector3Int &p_value)
					{
#ifndef NDEBUG
						_checkTypeValidity(Description::Descriptor::Uniform::Type::Vector3Int);
#endif
						_push(p_value);
					}

					void push(const Vector3UInt &p_value)
					{
#ifndef NDEBUG
						_checkTypeValidity(Description::Descriptor::Uniform::Type::Vector3UInt);
#endif
						_push(p_value);
					}

					void push(const Color& p_value)
					{
#ifndef NDEBUG
						_checkTypeValidity(Description::Descriptor::Uniform::Type::Vector4);
#endif
						_push(p_value);
					}

					void push(const Matrix4x4 &p_value)
					{
#ifndef NDEBUG
						_checkTypeValidity(Description::Descriptor::Uniform::Type::Matrix4x4);
#endif
						_push(p_value);
					}

					const std::wstring &name() const
					{
						return (_description.name);
					}

					const Description::Descriptor::Uniform &description() const
					{
						return (_description);
					}
				};

			private:
				Description::Descriptor _description;
				std::map<std::wstring, Uniform *> _uniforms;

				virtual Descriptor *_copy() const = 0;
				virtual Uniform *_allocateUniform(const Description::Descriptor::Uniform &p_description) = 0;

				void _loadUniforms()
				{
					for (size_t i = 0; i < _description.uniforms.size(); i++)
					{
						if (_uniforms.contains(_description.uniforms[i].name) == true)
							spk::throwException(L"Can't create a Uniform : Duplicated name found");

						Uniform *newUniform = _allocateUniform(_description.uniforms[i]);
						_uniforms[newUniform->name()] = newUniform;
					}
				}

			public:
				Descriptor(const Description::Descriptor &p_description) : _description(p_description)
				{
				}

				virtual void activate() = 0;
				virtual void deactivate() = 0;

				Descriptor *copy() const
				{
					Descriptor *result = _copy();
					result->_loadUniforms();
					return (result);
				}

				Uniform *uniform(const std::wstring &p_uniformName)
				{
					return (_uniforms[p_uniformName]);
				}

				const Description::Descriptor &description() const
				{
					return (_description);
				}
			};

		private:
			Description _description;
			Buffer *_datas;
			Buffer *_indexes;
			Descriptor *_descriptor;

			virtual Storage *_duplicate() = 0;
			virtual Buffer *_allocateBuffer(const Description::Buffer &p_bufferDescription) = 0;
			virtual Descriptor *_allocateDescriptor(const Description::Descriptor &p_bufferDescription) = 0;

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
				_descriptor = _allocateDescriptor(p_description.descriptor);
				_descriptor->_loadUniforms();
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
			Descriptor::Uniform *uniform(const std::wstring &p_uniformName)
			{
				return (_descriptor->uniform(p_uniformName));
			}
		};

	private:
	protected:
		Storage *_storage;

		std::vector<std::string> _parseBufferInformations(const std::string &p_shaderSource)
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
					std::smatch p_match = *p_it;

					std::string contact = p_match.str();
					p_result.push_back(contact);
				}
			}

			return p_result;
		}

		void _parseBuffer(const std::string &p_shaderCode, Storage::Description &p_descriptionToFill)
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
				{"uvec4", {4, Storage::Description::Buffer::Type::Float}}};

			static const std::unordered_map<Storage::Description::Buffer::Type, size_t> typeToSizeMap = {
				{Storage::Description::Buffer::Type::Float, sizeof(float)},
				{Storage::Description::Buffer::Type::UInt, sizeof(unsigned int)},
				{Storage::Description::Buffer::Type::Int, sizeof(int)}};

			std::vector<std::string> buffersInformations = _parseBufferInformations(p_shaderCode);

			uint32_t p_nextAvailableLocation = 0;

			for (size_t i = 0; i < buffersInformations.size(); i++)
			{
				std::smatch p_match;
				std::regex p_varPattern(R"((?:layout\s*?\(\s*?location\s*?=\s*?(\d+)\s*?\)\s+)?in\s+(\w+)\s+(\w+))");

				if (std::regex_search(buffersInformations[i], p_match, p_varPattern))
				{
					uint32_t p_location;
					if (p_match[1].length() > 0)
					{
						p_location = std::stoi(p_match[1]);
						p_usedLocations.insert(p_location);
					}
					else
					{
						while (p_usedLocations.find(p_nextAvailableLocation) != p_usedLocations.end())
						{
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
					if (glslInputToDataIter == glslInputToData.end())
					{
						spk::throwException(L"Unsupported data type [" + spk::to_wstring(p_type) + L"]");
					}

					p_attribute.format = std::get<0>(glslInputToDataIter->second);
					p_attribute.type = std::get<1>(glslInputToDataIter->second);

					p_attribute.offset = p_descriptionToFill.dataBuffer.stride;
					p_descriptionToFill.dataBuffer.stride += p_attribute.format * typeToSizeMap.at(p_attribute.type);

					p_descriptionToFill.dataBuffer.attributes.push_back(p_attribute);
				}
			}
		}

		std::vector<std::string> _parseDescriptorInformations(const std::string &p_shaderSource)
		{
			std::vector<std::string> p_result;

			std::stringstream p_stream(p_shaderSource);
			std::string p_line;

			std::regex p_pattern("uniform\\s+\\w+\\s+\\w+(\\s*\\[\\s*\\d+\\s*\\])?");

			while (std::getline(p_stream, p_line, ';'))
			{
				std::sregex_iterator p_it(p_line.begin(), p_line.end(), p_pattern);
				std::sregex_iterator p_end;

				for (; p_it != p_end; ++p_it)
				{
					std::smatch p_match = *p_it;

					std::string contact = p_match.str();
					p_result.push_back(contact);
				}
			}
			return p_result;
		}

		void _parseDescriptor(const std::string &p_shaderCode, Storage::Description &p_descriptionToFill)
		{
			static const std::unordered_map<std::wstring, Storage::Description::Descriptor::Uniform::Type> glslTypeToType 
				{
					{L"float", Storage::Description::Descriptor::Uniform::Type::Float},
					{L"int", Storage::Description::Descriptor::Uniform::Type::Int},
					{L"sampler1D", Storage::Description::Descriptor::Uniform::Type::Int},
					{L"sampler2D", Storage::Description::Descriptor::Uniform::Type::Int},
					{L"sampler3D", Storage::Description::Descriptor::Uniform::Type::Int},
					{L"samplerCube", Storage::Description::Descriptor::Uniform::Type::Int},
					{L"uint", Storage::Description::Descriptor::Uniform::Type::UInt},

					{L"vec2", Storage::Description::Descriptor::Uniform::Type::Vector2},
					{L"ivec2", Storage::Description::Descriptor::Uniform::Type::Vector2Int},
					{L"uvec2", Storage::Description::Descriptor::Uniform::Type::Vector2UInt},

					{L"vec3", Storage::Description::Descriptor::Uniform::Type::Vector3},
					{L"ivec3", Storage::Description::Descriptor::Uniform::Type::Vector3Int},
					{L"uvec3", Storage::Description::Descriptor::Uniform::Type::Vector3UInt},

					{L"vec4", Storage::Description::Descriptor::Uniform::Type::Vector4},

					{L"mat4", Storage::Description::Descriptor::Uniform::Type::Matrix4x4}
				};
			static const std::unordered_map<std::wstring, size_t> glslTypeToByteSize =
			{
				{L"float", sizeof(float)},
				{L"int", sizeof(int)},
				{L"uint", sizeof(unsigned int)},

				{L"vec2", sizeof(float) * 2},
				{L"ivec2", sizeof(int) * 2},
				{L"uvec2", sizeof(unsigned int) * 2},

				{L"vec3", sizeof(float) * 3},
				{L"ivec3", sizeof(int) * 3},
				{L"uvec3", sizeof(unsigned int) * 3},

				{L"vec4", sizeof(float) * 3},
				{L"ivec4", sizeof(int) * 3},
				{L"uvec4", sizeof(unsigned int) * 3},

				{L"mat4", sizeof(float) * 16}
			};

			std::vector<std::string> contacts = _parseDescriptorInformations(p_shaderCode);

			for (size_t i = 0; i < contacts.size(); i++)
			{
				std::vector<std::wstring> tab = spk::stringSplit(spk::to_wstring(contacts[i]), L" ");

				Storage::Description::Descriptor::Uniform newUniform;

				newUniform.type = glslTypeToType.at(tab[tab.size() - 2]);
				newUniform.size = glslTypeToByteSize.at(tab[tab.size() - 2]);
				newUniform.name = tab[tab.size() - 1];
				newUniform.offset = p_descriptionToFill.descriptor.uniformSize;
				p_descriptionToFill.descriptor.uniformSize += newUniform.size;
				p_descriptionToFill.descriptor.uniforms.push_back(newUniform);
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
				p_fragmentShaderPath.filename().string(), fragmentShaderContent);

			Storage::Description storageDescription;

			_parseBuffer(vertexShaderContent, storageDescription);

			_parseDescriptor(vertexShaderContent, storageDescription);
			_parseDescriptor(fragmentShaderContent, storageDescription);

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

	std::wstring to_wstring(const spk::AbstractGraphicalDevice::Storage::Description::Buffer::Type &p_input);
}