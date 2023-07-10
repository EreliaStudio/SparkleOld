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
			 * @brief Saves the JSON file to the specified path.
			 * @param p_filePath The path to save the JSON file to.
			 */
			void save(const std::filesystem::path& p_filePath) const;

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
