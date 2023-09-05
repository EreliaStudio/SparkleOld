#pragma once

#include <gl/glew.h>
#include <gl/wglew.h>
#include <string>
#include <filesystem>
#include "spk_basic_functions.hpp"
#include "math/spk_vector2.hpp"
#include "math/spk_vector3.hpp"
#include "math/spk_matrix4x4.hpp"

namespace spk
{
	class AbstractGraphicalDevice
	{
	public:
		class Storage
		{
		public:
			struct Element
			{
				enum class Type
				{
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

				Type type = Type::Data;
				size_t stride = 0;
				std::vector<Attribute> attributes = {};
			};

		protected:
			Element _elementDescription;

			template <typename T>
			struct isVectorType : std::false_type
			{
			};

			template <typename T, typename Alloc>
			struct isVectorType<std::vector<T, Alloc>> : std::true_type
			{
			};

			template <typename... Vectors,
					  typename std::enable_if_t<(... && isVectorType<Vectors>::value), int> = 0>
			void _validateVectorSizes(Vectors... p_vectors)
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

		public:
			Storage(const Element& p_elementDescription)
			{
				_elementDescription = p_elementDescription;
			}

			virtual Storage *copy() const = 0;

			virtual size_t nbElement() = 0;

			virtual void activate() = 0;
			virtual void deactivate() = 0;

			virtual void push(const spk::DataBuffer &p_bufferToPush) = 0;

			template <typename... Vectors,
					  typename std::enable_if_t<(... && isVectorType<Vectors>::value), int> = 0>
			void push(Vectors... p_vectors)
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

				push(dataBuffer);
			}

		};

		class AbstractUniform
		{
		public:
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

		private:
			Type _type;
			std::wstring _name;

			void _checkTypeValidity(const Type &p_pushingType)
			{
				if (_type != p_pushingType)
					spk::throwException(L"Error while pushing data to uniform [" + _name + L"] - Invalid type");
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
			AbstractUniform() :
				_type(Type::Error),
				_name(L"Unnamed")
			{

			}

			AbstractUniform(const std::wstring& p_name, const Type& p_type) :
				_type(p_type),
				_name(p_name)
			{

			}

			virtual AbstractUniform* copy() const = 0;

			void push(float p_value)
			{
#ifndef NDEBUG
				_checkTypeValidity(Type::Float);
#endif
				_push(p_value);
			}
			void push(int p_value)
			{
#ifndef NDEBUG
				_checkTypeValidity(Type::Int);
#endif
				_push(p_value);
			}

			void push(unsigned int p_value)
			{
#ifndef NDEBUG
				_checkTypeValidity(Type::UInt);
#endif
				_push(p_value);
			}

			void push(const Vector2 &p_value)
			{
#ifndef NDEBUG
				_checkTypeValidity(Type::Vec2);
#endif
				_push(p_value);
			}

			void push(const Vector2Int &p_value)
			{
#ifndef NDEBUG
				_checkTypeValidity(Type::Vec2Int);
#endif
				_push(p_value);
			}

			void push(const Vector2UInt &p_value)
			{
#ifndef NDEBUG
				_checkTypeValidity(Type::Vec2UInt);
#endif
				_push(p_value);
			}

			void push(const Vector3 &p_value)
			{
#ifndef NDEBUG
				_checkTypeValidity(Type::Vec3);
#endif
				_push(p_value);
			}

			void push(const Vector3Int &p_value)
			{
#ifndef NDEBUG
				_checkTypeValidity(Type::Vec3Int);
#endif
				_push(p_value);
			}

			void push(const Vector3UInt &p_value)
			{
#ifndef NDEBUG
				_checkTypeValidity(Type::Vec3UInt);
#endif
				_push(p_value);
			}

			void push(const Matrix4x4 &p_value)
			{
#ifndef NDEBUG
				_checkTypeValidity(Type::Mat4x4);
#endif
				_push(p_value);
			}

			void push(const std::vector<float> &p_values)
			{
#ifndef NDEBUG
				_checkTypeValidity(Type::FloatArray);
#endif
				_push(p_values);
			}

			void push(const std::vector<int> &p_values)
			{
#ifndef NDEBUG
				_checkTypeValidity(Type::IntArray);
#endif
				_push(p_values);
			}

			void push(const std::vector<unsigned int> &p_values)
			{
#ifndef NDEBUG
				_checkTypeValidity(Type::UIntArray);
#endif
				_push(p_values);
			}

			const std::wstring& name() const 
			{
				return (_name);
			}
		};

	protected:
		Storage *_dataStorage;
		Storage *_indexesStorage;
		std::map<std::wstring, AbstractUniform *> _uniforms;

		virtual Storage* _createStorage(const Storage::Element& p_element) = 0;
		virtual AbstractUniform* _createUniform(const std::string& p_inputLine) = 0;

		virtual void _loadDevice(
			const std::string &p_vertexModuleName, const std::string &p_vertexModuleCode,
			const std::string &p_fragmentModuleName, const std::string &p_fragmentModuleCode) = 0;

		void _parseBuffer(const std::string& p_shaderCode);
		void _parseUniform(const std::string& p_shaderCode);

	public:
		AbstractGraphicalDevice();

		void load(const std::wstring &p_vertexShaderCode, const std::wstring &p_fragmentShaderCode);
		void loadFromFile(const std::filesystem::path &p_vertexShaderPath, const std::filesystem::path &p_fragmentShaderPath);

		virtual void activate() = 0;
		virtual void launch(const size_t& p_nbIndexes) = 0;

		Storage *dataStorage();
		Storage *indexesStorage();
		std::map<std::wstring, AbstractUniform *> &uniforms();
		AbstractUniform *uniform(const std::wstring &p_uniformName);
	};
}