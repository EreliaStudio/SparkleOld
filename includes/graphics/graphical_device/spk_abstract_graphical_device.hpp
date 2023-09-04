#pragma once

#include <gl/glew.h>
#include <gl/wglew.h>
#include <string>
#include <filesystem>
#include "spk_basic_functions.hpp"

namespace spk
{
	class AbstractGraphicalDevice
	{
	public:
		class AbstractStorage
		{
		private:
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
				spk::cout << "Adding data [" << p_vector[p_index] << "]" << std::endl;
				p_dataBuffer << p_vector[p_index];
			}

		public:
			virtual AbstractStorage *copy() const = 0;

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

			void _checkTypeValidity(const Type &p_pushingType)
			{
				if (_type != p_pushingType)
					spk::throwException(L"Error while pushing data to uniform [" + +L"] - Invalid type");
			}

			virtual void _push(float p_value) = 0;
			virtual void _push(int p_value) = 0;
			virtual void _push(unsigned int p_value) = 0;

			virtual void _push(const Vector2 &p_value) = 0;
			virtual void _push(const Vector2Int &p_value) = 0;
			virtual void _push(const Vector2Uint &p_value) = 0;

			virtual void _push(const Vector3 &p_value) = 0;
			virtual void _push(const Vector3Int &p_value) = 0;
			virtual void _push(const Vector3Uint &p_value) = 0;

			virtual void _push(const Matrix4x4 &p_value) = 0;

			virtual void _push(const std::vector<float> &p_values) = 0;
			virtual void _push(const std::vector<int> &p_values) = 0;
			virtual void _push(const std::vector<unsigned int> &p_values) = 0;

		public:
			AbstractUniform(const Type& p_type) :
				_type(p_type)
			{

			}

			AbstractUniform* copy() const = 0;

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

			void push(const Vector2Uint &p_value)
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

			void push(const Vector3Uint &p_value)
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
		};

	protected:
		Storage *_elementStorage;
		Storage *_indexesStorage;
		std::map<std::wstring, AbstractUniform *> _uniforms;

		virtual void _loadDevice(
			const std::string &p_vertexModuleName, const std::string &p_vertexModuleCode,
			const std::string &p_fragmentModuleName, const std::string &p_fragmentModuleCode) = 0;

	public:
		AbstractGraphicalDevice();

		void load(const std::wstring &p_vertexShaderCode, const std::wstring &p_fragmentShaderCode);
		void loadFromFile(const std::filesystem::path &p_vertexShaderPath, const std::filesystem::path &p_fragmentShaderPath);

		AbstractStorage *elementStorage();
		AbstractStorage *indexesStorage();
		std::map<std::wstring, AbstractUniform *> &uniforms();
		AbstractUniform *uniform(const std::wstring &p_uniformName);
	};
}