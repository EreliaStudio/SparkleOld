#pragma once

#include "miscellaneous/JSON/spk_JSON_object.hpp"
#include "spk_basic_functions.hpp"
#include <filesystem>
#include <cfenv>
#include <cmath>

namespace spk
{
	namespace JSON
	{
		/**
		 * @brief Represents a JSON file and provides methods to load and access its contents.
		 */
		class File
		{
		private:
			spk::JSON::Object _root; /**< The root JSON object of the file. */

			/**
			 * @brief Applies grammar rules to the file content to ensure valid JSON syntax.
			 * @param p_fileContent The content of the file to be modified.
			 */
			void _applyGrammar(std::wstring &p_fileContent);

			/**
			 * @brief Loads the content of the file at the specified path.
			 * @param p_filePath The path to the JSON file.
			 * @return The loaded content of the file as a wide string.
			 */
			std::wstring _loadFileContent(const std::filesystem::path &p_filePath);

			/**
			 * @brief Extracts the attribute name from the content at the specified index.
			 * @param p_content The content string.
			 * @param p_index The current index in the content string. It will be updated to the index after the attribute name.
			 * @return The extracted attribute name.
			 */
			std::wstring _getAttributeName(const std::wstring &p_content, size_t &p_index);

			/**
			 * @brief Extracts the unit substring from the content at the specified index.
			 * @param p_content The content string.
			 * @param p_index The current index in the content string. It will be updated to the index after the unit substring.
			 * @return The extracted unit substring.
			 */
			std::wstring _extractUnitSubstring(const std::wstring &p_content, size_t &p_index);

			/**
			 * @brief Loads the unit string into the specified JSON object.
			 * @param p_objectToFill The JSON object to fill with the unit string.
			 * @param p_unitSubString The unit substring to load.
			 */
			void _loadUnitString(spk::JSON::Object &p_objectToFill, const std::wstring &p_unitSubString);

			/**
			 * @brief Loads the unit numbers into the specified JSON object.
			 * @param p_objectToFill The JSON object to fill with the unit numbers.
			 * @param p_unitSubString The unit substring to load.
			 */
			void _loadUnitNumbers(spk::JSON::Object &p_objectToFill, const std::wstring &p_unitSubString);

			/**
			 * @brief Loads the unit boolean value into the specified JSON object.
			 * @param p_objectToFill The JSON object to fill with the unit boolean.
			 * @param p_unitSubString The unit substring to load.
			 */
			void _loadUnitBoolean(spk::JSON::Object &p_objectToFill, const std::wstring &p_unitSubString);

			/**
			 * @brief Loads the unit null value into the specified JSON object.
			 * @param p_objectToFill The JSON object to fill with the unit null.
			 * @param p_unitSubString The unit substring to load.
			 */
			void _loadUnitNull(spk::JSON::Object &p_objectToFill, const std::wstring &p_unitSubString);

			/**
			 * @brief Loads the unit value into the specified JSON object.
			 * @param p_objectToFill The JSON object to fill with the unit value.
			 * @param p_content The content string.
			 * @param p_index The current index in the content string. It will be updated to the index after loading the unit value.
			 */
			void _loadUnit(spk::JSON::Object &p_objectToFill, const std::wstring &p_content, size_t &p_index);

			/**
			 * @brief Loads the JSON object into the specified JSON object.
			 * @param p_objectToFill The JSON object to fill with the nested object.
			 * @param p_content The content string.
			 * @param p_index The current index in the content string. It will be updated to the index after loading the object.
			 */
			void _loadObject(spk::JSON::Object &p_objectToFill, const std::wstring &p_content, size_t &p_index);

			/**
			 * @brief Loads the JSON array into the specified JSON object.
			 * @param p_objectToFill The JSON object to fill with the array.
			 * @param p_content The content string.
			 * @param p_index The current index in the content string. It will be updated to the index after loading the array.
			 */
			void _loadArray(spk::JSON::Object &p_objectToFill, const std::wstring &p_content, size_t &p_index);

			/**
			 * @brief Loads the JSON content into the specified JSON object.
			 * @param p_objectToFill The JSON object to fill with the content.
			 * @param p_content The content string.
			 * @param p_index The current index in the content string. It will be updated to the index after loading the content.
			 */
			void _loadContent(spk::JSON::Object &p_objectToFill, const std::wstring &p_content, size_t &p_index);

		public:
			/**
			 * @brief Default constructor for the File class.
			 */
			File();

			/**
			 * @brief Constructor for the File class that loads the specified JSON file.
			 * @param p_filePath The path to the JSON file.
			 */
			File(const std::filesystem::path& p_filePath);

			/**
			 * @brief Loads the specified JSON file.
			 * @param p_filePath The path to the JSON file.
			 */
			void load(const std::filesystem::path& p_filePath);

			/**
			 * @brief Accesses the JSON object value with the specified key.
			 * @param p_key The key to access the JSON object value.
			 * @return The JSON object value with the specified key.
			 */
			const spk::JSON::Object &operator[](const std::wstring &p_key) const;

			/**
			 * @brief Accesses the JSON object value at the specified index.
			 * @param p_index The index to access the JSON object value.
			 * @return The JSON object value at the specified index.
			 */
			const spk::JSON::Object &operator[](size_t p_index) const;

			/**
			 * @brief Converts the root JSON object to the specified type.
			 * @tparam TType The type to convert the root JSON object to.
			 * @return The root JSON object as the specified type.
			 */
			template <typename TType>
			const TType &as()
			{
				return (_root.as<TType>());
			}

			/**
			 * @brief Returns the root JSON object.
			 * @return The root JSON object.
			 */
			const spk::JSON::Object &root() const;

			/**
			 * @brief Overloaded stream insertion operator to output the contents of the File object to an output stream.
			 * @param p_os The output stream to write the File object to.
			 * @param p_file The File object to output.
			 * @return The modified output stream.
			 */
			friend std::wostream &operator<<(std::wostream &p_os, const File &p_file);
		};
	}
}
