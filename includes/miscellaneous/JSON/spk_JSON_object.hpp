#pragma once

#include <variant>
#include <map>
#include <vector>

#include "iostream/spk_iostream.hpp"

namespace spk
{
	namespace JSON
	{
		class Object
		{
		public:
			using Unit = std::variant<bool, long, double, std::wstring, Object *, std::nullptr_t>;
			using ContentType = std::variant<Unit, std::map<std::wstring, Object *>, std::vector<Object *>>;

		private:
			bool _initialized;
			ContentType _content;

		public:
			Object();

			void reset();

			Object &addAttribute(const std::wstring &p_key);

			Object &operator[](const std::wstring &p_key);
			const Object &operator[](const std::wstring &p_key) const;

			Object &append();
			void push_back(Object &p_object);
			Object &operator[](size_t p_index);
			const Object &operator[](size_t p_index) const;

			size_t size() const;
			size_t count(const std::wstring &p_key) const;

			template <typename TType,
					  typename std::enable_if<!std::is_same<TType, Object>::value, int>::type = 0>
			void set(const TType &p_value)
			{
				if (_initialized == false)
				{
					_content = Unit();
					_initialized = true;
				}

				std::get<Unit>(_content) = p_value;
			}

			template <typename TType,
					  typename std::enable_if<std::is_same<TType, Object>::value, int>::type = 0>
			void set(const TType &p_value)
			{
				Object *tmpObject = new Object(p_value);
				set<Object *>(tmpObject);
			}

			template <typename TType>
			const TType &as() const
			{
				return (std::get<TType>(std::get<Unit>(_content)));
			}

			void printUnit(std::wostream &p_os) const;
			void printObject(std::wostream &p_os) const;
			void printArray(std::wostream &p_os) const;

			friend std::wostream &operator<<(std::wostream &p_os, const Object &p_object);
		};
	}
}