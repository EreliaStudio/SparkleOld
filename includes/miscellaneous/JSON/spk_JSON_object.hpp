#pragma once

#include <variant>
#include <map>
#include <vector>
#include <iomanip>
#include <algorithm>

#include "iostream/spk_iostream.hpp"

namespace spk
{
	namespace JSON
	{
		/**
		 * @brief Represents a JSON object that can store various types of values.
		 */
		class Object
		{
		public:
			using Unit = std::variant<bool, long, double, std::wstring, Object*, std::nullptr_t>; /**< The type for a single JSON value. */
			using ContentType = std::variant<Unit, std::map<std::wstring, Object*>, std::vector<Object*>>; /**< The type for the content of the JSON object. */

		private:
			bool _initialized; /**< Flag indicating whether the object has been initialized. */
			ContentType _content; /**< The content of the JSON object. */
			static size_t _indent; /**< The indentation level of the JSON object. */
			const static uint8_t _indentSize = 4; /**< The size of the indentation. */

		public:
			/**
			 * @brief Default constructor for the Object class.
			 */
			Object();

			/**
			 * @brief Resets the object to its default state.
			 */
			void reset();

			/**
			 * @brief Adds an attribute to the JSON object with the specified key.
			 * @param p_key The key of the attribute to add.
			 * @return Reference to the JSON object itself.
			 */
			Object& addAttribute(const std::wstring& p_key);

			/**
			 * @brief Accesses the JSON object or value with the specified key.
			 * @param p_key The key to access the JSON object or value.
			 * @return Reference to the JSON object or value with the specified key.
			 */
			Object& operator[](const std::wstring& p_key);

			/**
			 * @brief Accesses the JSON object or value with the specified key (const version).
			 * @param p_key The key to access the JSON object or value.
			 * @return Reference to the JSON object or value with the specified key.
			 */
			const Object& operator[](const std::wstring& p_key) const;

			/**
			 * @brief Get the map of elements in the JSON object.
			 */
			std::map<std::wstring, Object*> getObjectElements() const;

			/**
			 * @brief Sets the JSON object as an object type.
			 */
			void setAsObject();

			/**
			 * @brief Appends an empty JSON object to the JSON array.
			 * @return Reference to the appended JSON object.
			 */
			Object& append();

			/**
			 * @brief Adds a JSON object to the JSON array.
			 * @param p_object The JSON object to add.
			 */
			void push_back(Object& p_object);

			/**
			 * @brief Accesses the JSON object or value at the specified index.
			 * @param p_index The index to access the JSON object or value.
			 * @return Reference to the JSON object or value at the specified index.
			 */
			Object& operator[](size_t p_index);

			/**
			 * @brief Accesses the JSON object or value at the specified index (const version).
			 * @param p_index The index to access the JSON object or value.
			 * @return Reference to the JSON object or value at the specified index.
			 */
			const Object& operator[](size_t p_index) const;

			/**
			 * @brief Sets the JSON object as an array type.
			 */
			void setAsArray();

			/**
			 * @brief Returns the number of attributes or elements in the JSON object.
			 * @return The number of attributes or elements in the JSON object.
			 */
			size_t size() const;

			/**
			 * @brief Returns the number of occurrences of the specified key in the JSON object.
			 * @param p_key The key to count occurrences.
			 * @return The number of occurrences of the specified key in the JSON object.
			 */
			size_t count(const std::wstring& p_key) const;

			/**
			 * @brief Sets the JSON object value to the specified type and value.
			 * @tparam TType The type of the value.
			 * @param p_value The value to set.
			 * @note This function is enabled for non-Object types.
			 */
			template <typename TType,
				typename std::enable_if<!std::is_same<TType, Object>::value, int>::type = 0>
			void set(const TType& p_value)
			{
				if (_initialized == false)
				{
					_content = Unit();
					_initialized = true;
				}

				std::get<Unit>(_content) = p_value;
			}

			/**
			 * @brief Sets the JSON object value to the specified Object type and value.
			 * @tparam TType The type of the value.
			 * @param p_value The value to set.
			 * @note This function is enabled for Object type.
			 */
			template <typename TType,
				typename std::enable_if<std::is_same<TType, Object>::value, int>::type = 0>
			void set(const TType& p_value)
			{
				Object* tmpObject = new Object(p_value);
				set<Object*>(tmpObject);
			}

			/**
			 * @brief Returns the JSON object value as the specified type.
			 * @tparam TType The type to convert the JSON object value to.
			 * @return The JSON object value as the specified type.
			 */
			template <typename TType>
			const TType& as() const
			{
				return (std::get<TType>(std::get<Unit>(_content)));
			}

			/**
			 * @brief Prints the JSON unit value to the output stream.
			 * @param p_os The output stream to write the JSON unit value to.
			 */
			void printUnit(std::wostream& p_os) const;

			/**
			 * @brief Prints the JSON object to the output stream.
			 * @param p_os The output stream to write the JSON object to.
			 */
			void printObject(std::wostream& p_os) const;

			/**
			 * @brief Prints the JSON array to the output stream.
			 * @param p_os The output stream to write the JSON array to.
			 */
			void printArray(std::wostream& p_os) const;

			/**
			 * @brief Overloaded stream insertion operator to output the contents of the Object to an output stream.
			 * @param p_os The output stream to write the Object to.
			 * @param p_object The Object to output.
			 * @return The modified output stream.
			 */
			friend std::wostream& operator<<(std::wostream& p_os, const Object& p_object);
		};
	}
}
