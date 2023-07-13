#pragma once

#include <unordered_map>
#include "miscellaneous/JSON/spk_JSON_file.hpp"
#include <unordered_map>

namespace spk
{
	/**
	 * @class TranslationAtlas
	 * @brief Class used to store localized strings.
	 * 
	 * @note The localization file to load must be a JSON file with the following format:
	 * @code
	 * {
	 * 	"key": "value",
	 * 	"key2": "value2",
	 * 	"...": "...",
	 * 	"keyN": "valueN"
	 * }
	 * @endcode
	 */
	class TranslationAtlas
	{
	private:
		std::unordered_map<std::wstring, std::wstring> _translations; ///< Map containing all translations.

	public:
		/**
		 * @brief Construct a new Translation Atlas object
		 */
		TranslationAtlas() = default;

		/**
		 * @brief Construct a new Translation Atlas object and load the specified file.
		 * 
		 * @param p_path Path to the file to load.
		 */
		TranslationAtlas(const std::filesystem::path& p_path);

		/**
		 * @brief Destroy the Translation Atlas object
		 */
		~TranslationAtlas() = default;

		/**
		 * @brief It is forbidden to copy a TranslationAtlas object.
		 */
		TranslationAtlas(const TranslationAtlas&) = delete;

		/**
		 * @brief It is forbidden to assign a TranslationAtlas object.
		 */
		TranslationAtlas& operator=(const TranslationAtlas&) = delete;

		/**
		 * @brief Loads the specified JSON file.
		 * 
		 * @param p_path Path to the file to load.
		 * @throw std::runtime_error if the file cannot be loaded.
		 */
		void load(const std::filesystem::path& p_path);

		/**
		 * @brief Returns the localized string corresponding to the specified key.
		 * 
		 * @param p_key The key of the localized string to get.
		 * @return The localized string corresponding to the specified key.
		 * @note If the key is not found, the key itself is returned.
		 */
		const std::wstring& get(const std::wstring& p_key) const;
	};
}