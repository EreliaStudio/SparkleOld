#include "miscellaneous/JSON/spk_JSON_file.hpp"

#include <fstream>
#include <algorithm>

#include "spk_basic_functions.hpp"

namespace spk
{
	namespace JSON
	{
		std::wstring _applyGrammar(const std::wstring& p_fileContent);
		std::wstring _extractUnitSubstring(const std::wstring& p_content, size_t& p_index);
		std::wstring _getAttributeName(const std::wstring& p_content, size_t& p_index);

		void _loadUnitNumbers(spk::JSON::Object& p_objectToFill, const std::wstring& p_unitSubString);

		static void _loadContent(spk::JSON::Object& p_objectToFill, const std::wstring& p_content, size_t& p_index);

		/**
		 * @brief Loads the content of the file at the specified path.
		 * @param p_filePath The path to the JSON file.
		 * @return The loaded content of the file as a wide string.
		 */
		static std::wstring _loadFileContent(const std::filesystem::path& p_filePath)
		{
			std::wifstream wif;

			wif.imbue(std::locale(""));
			wif.open(p_filePath);
			if (!wif.is_open())
			{
				spk::throwException(L"Failed to open file [" + p_filePath.wstring() + L"]");
			}

			std::wstring result((std::istreambuf_iterator<wchar_t>(wif)),
				std::istreambuf_iterator<wchar_t>());
			wif.close();

			return (_applyGrammar(result));
		}

		/**
		 * @brief Loads the unit string into the specified JSON object.
		 * @param p_objectToFill The JSON object to fill with the unit string.
		 * @param p_unitSubString The unit substring to load.
		 */
		static void _loadUnitString(spk::JSON::Object& p_objectToFill, const std::wstring& p_unitSubString)
		{
			p_objectToFill.set(p_unitSubString.substr(1, p_unitSubString.size() - 2));
		}

		/**
		 * @brief Loads the unit boolean value into the specified JSON object.
		 * @param p_objectToFill The JSON object to fill with the unit boolean.
		 * @param p_unitSubString The unit substring to load.
		 */
		static void _loadUnitBoolean(spk::JSON::Object& p_objectToFill, const std::wstring& p_unitSubString)
		{
			if (p_unitSubString == L"true")
				p_objectToFill.set(true);
			else if (p_unitSubString == L"false")
				p_objectToFill.set(false);
			else
				spk::throwException(L"Invalid boolean JSON value: " + p_unitSubString);
		}

		/**
		 * @brief Loads the unit null value into the specified JSON object.
		 * @param p_objectToFill The JSON object to fill with the unit null.
		 * @param p_unitSubString The unit substring to load.
		 */
		static void _loadUnitNull(spk::JSON::Object& p_objectToFill, const std::wstring& p_unitSubString)
		{
			if (p_unitSubString == L"null")
				p_objectToFill.set(nullptr);
			else
				spk::throwException(L"Invalid null JSON value: " + p_unitSubString);
		}

		/**
		 * @brief Loads the unit value into the specified JSON object.
		 * @param p_objectToFill The JSON object to fill with the unit value.
		 * @param p_content The content string.
		 * @param p_index The current index in the content string. It will be updated to the index after loading the unit value.
		 */
		static void _loadUnit(spk::JSON::Object& p_objectToFill, const std::wstring& p_content, size_t& p_index)
		{
			std::wstring substring = _extractUnitSubstring(p_content, p_index);

			switch (substring[0])
			{
			case '"':
				_loadUnitString(p_objectToFill, substring);
				break;
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
			case '-':
				_loadUnitNumbers(p_objectToFill, substring);
				break;
			case 't': case 'f':
				_loadUnitBoolean(p_objectToFill, substring);
				break;
			case 'n':
				_loadUnitNull(p_objectToFill, substring);
				break;
			default:
				spk::throwException(L"Invalid JSON value: " + substring);
				break;
			}
		}

		/**
		 * @brief Loads the JSON object into the specified JSON object.
		 * @param p_objectToFill The JSON object to fill with the nested object.
		 * @param p_content The content string.
		 * @param p_index The current index in the content string. It will be updated to the index after loading the object.
		 */
		static void _loadObject(spk::JSON::Object& p_objectToFill, const std::wstring& p_content, size_t& p_index)
		{
			p_objectToFill.setAsObject();

			if (p_content[p_index] != '{')
				spk::throwException(L"Invalid JSON object (missing '{')");
			p_index++;
			for (; p_index < p_content.size() && p_content[p_index] != '}';)
			{
				std::wstring attributeName = _getAttributeName(p_content, p_index);

				spk::JSON::Object& newObject = p_objectToFill.addAttribute(attributeName);

				_loadContent(newObject, p_content, p_index);

				if (p_content[p_index] == ',')
					p_index++;
				else if (p_content[p_index] != '}')
					spk::throwException(L"Invalid JSON object (missing ',' or '}')");
			}
			if (p_content[p_index] != '}')
				spk::throwException(L"Invalid JSON object (missing '}')");
			p_index++;
		}

		/**
		 * @brief Loads the JSON array into the specified JSON object.
		 * @param p_objectToFill The JSON object to fill with the array.
		 * @param p_content The content string.
		 * @param p_index The current index in the content string. It will be updated to the index after loading the array.
		 */
		static void _loadArray(spk::JSON::Object& p_objectToFill, const std::wstring& p_content, size_t& p_index)
		{
			p_objectToFill.setAsArray();
			if (p_content[p_index] != '[')
				spk::throwException(L"Invalid JSON array (missing '[')");
			p_index++;
			for (; p_index < p_content.size() && p_content[p_index] != ']';)
			{
				spk::JSON::Object newObject(L"[" + std::to_wstring(p_objectToFill.size()) + L"]");

				_loadContent(newObject, p_content, p_index);

				p_objectToFill.push_back(newObject);

				if (p_content[p_index] == ',')
					p_index++;
				else if (p_content[p_index] != ']')
					spk::throwException(L"Invalid JSON array (missing ',' or ']')");
			}
			if (p_content[p_index] != ']')
				spk::throwException(L"Invalid JSON array (missing ']')");
			p_index++;
		}

		/**
		 * @brief Loads the JSON content into the specified JSON object.
		 * @param p_objectToFill The JSON object to fill with the content.
		 * @param p_content The content string.
		 * @param p_index The current index in the content string. It will be updated to the index after loading the content.
		 */
		static void _loadContent(spk::JSON::Object& p_objectToFill, const std::wstring& p_content, size_t& p_index)
		{
			switch (p_content[p_index])
			{
			case '\"':
			case 'f': case 't': case 'n':
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
			case '-':
				_loadUnit(p_objectToFill, p_content, p_index);
				break;
			case '{':
				_loadObject(p_objectToFill, p_content, p_index);
				break;
			case '[':
				_loadArray(p_objectToFill, p_content, p_index);
				break;
			default:
				spk::throwException(L"Unexpected data type in JSON: " + p_content.substr(p_index, 10));
			}
		}

		File::File() :
			_root(L"Root")
		{
		}

		File::File(const std::filesystem::path& p_filePath) :
			_root(L"Root")
		{
			load(p_filePath);
		}

		void File::load(const std::filesystem::path& p_filePath)
		{
			std::wstring fileContent = _loadFileContent(p_filePath);
			size_t index = 0;

			if (fileContent.empty())
				spk::throwException(L"Empty file: " + p_filePath.wstring());
			_root.reset();
			_loadContent(_root, fileContent, index);
			if (index != fileContent.size())
				spk::throwException(L"Invalid JSON file: " + p_filePath.wstring());
		}

		void File::save(const std::filesystem::path& p_filePath) const
		{
			std::wofstream file;

			file.imbue(std::locale(""));
			file.open(p_filePath);
			if (file.is_open() == false)
				spk::throwException(L"Unable to open file: " + p_filePath.wstring());
			file << _root;
			file.close();
		}
		
		bool File::contains(const std::wstring& p_key) const
		{
			return (_root.contains(p_key));
		}

		const spk::JSON::Object& File::operator[](const std::wstring& p_key) const
		{
			return (_root[p_key]);
		}

		const spk::JSON::Object& File::operator[](size_t p_index) const
		{
			return (_root[p_index]);
		}

		const spk::JSON::Object& File::root() const
		{
			return (_root);
		}

		std::wostream& operator<<(std::wostream& p_os, const File& p_file)
		{
			p_os << p_file._root;
			return (p_os);
		}
	}
}