#include "miscellaneous/JSON/spk_JSON_file.hpp"

#include <fstream>
#include <algorithm>
#include <cwctype>

#include "spk_basic_functions.hpp"

namespace spk
{
	namespace JSON
	{
		static std::wstring _handleEscapeSequence(const std::wstring& p_fileContent)
		{
			std::wstring result;

			for (size_t i = 0; i < p_fileContent.size(); ++i)
			{
				if (p_fileContent[i] == L'\\')
				{
					switch (p_fileContent[i + 1])
					{
					case L'"':
						result += L'"';
						break;
					case L'\\':
						result += L"\\";
						break;
					case L'/':
						result += L'/';
						break;
					case L'b':
						result += L'\b';
						break;
					case L'f':
						result += L'\f';
						break;
					case L'n':
						result += L'\n';
						break;
					case L'r':
						result += L'\r';
						break;
					case L't':
						result += L'\t';
						break;
					case L'u':
						try
						{
							result += static_cast<wchar_t>(std::stoi(p_fileContent.substr(i + 2, 4), nullptr, 16));
						}
						catch (const std::exception& e)
						{
							spk::throwException(L"Invalid Unicode escape : <" + p_fileContent.substr(i, 6) + L'>');
						}
						i += 4;
						break;
					default:
						spk::throwException(L"Invalid escape sequence: <" + p_fileContent.substr(i, 2) + L'>');
					}
					++i;
				}
				else
					result += p_fileContent[i];
			}
			return (result);
		}

		/**
		 * @brief Applies grammar rules to the file content to ensure valid JSON syntax.
		 * @param p_fileContent The content of the file to be modified.
		 */
		static void _applyGrammar(std::wstring& p_fileContent)
		{
			std::wstring result;
			bool isLiteral(false);

			for (size_t i(0); i < p_fileContent.size(); ++i)
			{
				wchar_t c = p_fileContent[i];
				wchar_t next_c = p_fileContent[i + 1];

				switch (c)
				{
				case L'\\':
					if (std::wstring(L"\"\\/bfnrtu").find(next_c) == std::wstring::npos)
						spk::throwException(L"Invalid escape sequence at line " +
							std::to_wstring(1 + std::count(p_fileContent.begin(), p_fileContent.begin() + i, '\n')) +
							L" column " + std::to_wstring(1 + i - p_fileContent.rfind('\n', i)) + L".");
					if (next_c == L'\\')
					{
						result += L"\\";
						++i;
					}
					result += c;
					break;
				case L'\"':
					if (i == 0 || (i >= 1 && p_fileContent[i - 1] != L'\\') ||
						(i >= 2 && p_fileContent[i - 1] == L'\\' && p_fileContent[i - 2] == L'\\'))
						isLiteral = !isLiteral;
					result += c;
					break;
				case L' ': case L'\t': case L'\n': case L'\r':
					if (isLiteral == true && c != L'\n' && c != L'\r')
						result += c;
					else if (isLiteral == true && (c == L'\n' || c == L'\r'))
						spk::throwException(L"Unexpected end of string " +
							std::to_wstring(1 + std::count(p_fileContent.begin(), p_fileContent.begin() + i, '\n')) +
							L" column " + std::to_wstring(i - p_fileContent.rfind('\n', i - 1)) + L".");
					break;
				default:
					result += c;
				}
			}
			p_fileContent = result;
		}

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
				throw std::runtime_error("Failed to open file");
			}

			std::wstring result((std::istreambuf_iterator<wchar_t>(wif)),
				std::istreambuf_iterator<wchar_t>());
			wif.close();

			_applyGrammar(result);
			return (result);
		}

		/**
		 * @brief Extracts the attribute name from the content at the specified index.
		 * @param p_content The content string.
		 * @param p_index The current index in the content string. It will be updated to the index after the attribute name.
		 * @return The extracted attribute name.
		 */
		static std::wstring _getAttributeName(const std::wstring& p_content, size_t& p_index)
		{
			if (p_content[p_index] != L'"')
				spk::throwException(L"Invalid attribute name [" + p_content.substr(p_index, 2) + L"]");

			++p_index;
			size_t start = p_index;

			for (; p_index < p_content.size() && p_content[p_index] != '"'; p_index++)
			{
				if (p_content[p_index] == '\\' && p_content[p_index + 1] != '"')
					++p_index;
			}
			++p_index;
			if (p_content[p_index] != ':')
				spk::throwException(L"Invalid attribute name [" + p_content.substr(start, p_index - start) + L"]");
			++p_index;
			return (_handleEscapeSequence(p_content.substr(start, p_index - start - 2)));
		}

		/**
		 * @brief Extracts the unit substring from the content at the specified index.
		 * @param p_content The content string.
		 * @param p_index The current index in the content string. It will be updated to the index after the unit substring.
		 * @return The extracted unit substring.
		 */
		static std::wstring _extractUnitSubstring(const std::wstring& p_content, size_t& p_index)
		{
			size_t oldIndex = p_index;
			bool isAString = false;

			if (p_content[p_index] == '"')
			{
				isAString = true;
				++p_index;
			}
			for (; p_index < p_content.size() && ((p_content[p_index] != ',' &&
				p_content[p_index] != '}' && p_content[p_index] != ']') ||
				isAString == true);
				p_index++)
			{
				if (isAString == true &&
					(p_index >= 1 && p_content[p_index] == '"' && p_content[p_index - 1] != '\\') ||
					(p_index >= 2 && p_content[p_index] == '"' && p_content[p_index - 1] == '\\' && p_content[p_index - 2] == '\\'))
				{
					break;
				}
				if (isAString == false &&
					(p_content[p_index] == '"' || p_content[p_index] == '[' ||
						p_content[p_index] == '{' || p_content[p_index] == ','))
					spk::throwException(L"Invalid JSON unit [" + p_content.substr(oldIndex, 5) + L"]");
			}
			if (isAString == true)
			{
				if (p_content[p_index] != '"')
					spk::throwException(L"Invalid JSON unit [" + p_content.substr(oldIndex, p_index - oldIndex) + L"]");
				else
					++p_index;
			}

			return (_handleEscapeSequence(p_content.substr(oldIndex, p_index - oldIndex)));
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

		static bool _isNumberMalformatted(bool p_isNegative, const size_t& p_decimalPos,
			const size_t& p_exponentPos, const std::wstring& p_unitSubString)
		{
			return (p_exponentPos == p_unitSubString.size() - 1 ||
				(p_isNegative == true && p_unitSubString.size() == 1) ||
				(p_isNegative == true && p_unitSubString.size() > 1 && ::isdigit(p_unitSubString[1]) == false) ||
				p_decimalPos == p_unitSubString.size() - 1 ||
				(p_decimalPos != std::wstring::npos && ::isdigit(p_unitSubString[p_decimalPos + 1]) == false) ||
				(p_decimalPos != std::wstring::npos && p_exponentPos != std::wstring::npos && p_decimalPos > p_exponentPos) ||
				(p_unitSubString[p_isNegative] == L'0' && p_unitSubString.size() > p_isNegative + 1 &&
					std::wstring(L".eE").find(p_unitSubString[p_isNegative + 1]) == std::wstring::npos) ||
				std::count(p_unitSubString.begin(), p_unitSubString.end(), L'.') > 1);
		}

		static long _extractExponent(const std::wstring& p_exponentSubstring)
		{
			bool isExponentSigned(p_exponentSubstring[0] == L'-' || p_exponentSubstring[0] == L'+');
			long result(0);

			if (p_exponentSubstring.find_first_not_of(L"0123456789", isExponentSigned) != std::wstring::npos)
				spk::throwException(L"Invalid numbers JSON exponent value: " + p_exponentSubstring);
			try
			{
				result = std::stol(p_exponentSubstring);
			}
			catch (const std::exception& e)
			{
				spk::throwException(L"Invalid numbers JSON value: " + p_exponentSubstring + L" too big (number overflow)");
			}
			return (result);
		}

		static bool _resultWillBeDouble(const size_t& p_decimalPos, bool p_hasExponent, const size_t& p_exponentPos,
			bool p_isNegative, const long& p_exponent)
		{
			if (p_decimalPos == std::wstring::npos && p_exponentPos == false)
				return false;
			return (p_decimalPos != std::wstring::npos || (p_hasExponent == true &&
				p_exponent > spk::numberLength(std::numeric_limits<long>::max()) ||
				p_exponentPos - p_isNegative > spk::numberLength(std::numeric_limits<long>::max()) ||
				p_exponentPos - p_isNegative + p_exponent <= 0));
		}

		static long _safePowerOfTen(const long& p_number, const long& p_exponent, const std::wstring& p_unitSubString)
		{
			long result(0);
			errno = 0;
			std::feclearexcept(FE_ALL_EXCEPT);

			result = p_number * std::pow(10, p_exponent);

			if (errno == EDOM || errno == ERANGE || std::fetestexcept(FE_ALL_EXCEPT ^ FE_INEXACT) != 0)
				spk::throwException(L"Invalid numbers JSON value: " + p_unitSubString + L" too big (power overflow)");

			return (result);
		}

		/**
		 * @brief Loads the unit numbers into the specified JSON object.
		 * @param p_objectToFill The JSON object to fill with the unit numbers.
		 * @param p_unitSubString The unit substring to load.
		 */
		static void _loadUnitNumbers(spk::JSON::Object& p_objectToFill, const std::wstring& p_unitSubString)
		{
			std::variant<long, double> result;

			bool isNegative(p_unitSubString[0] == L'-');

			size_t exponentPos(p_unitSubString.find_first_of(L"eE"));
			long exponent(0);

			size_t decimalPos(p_unitSubString.find_last_of(L"."));

			if (_isNumberMalformatted(isNegative, decimalPos, exponentPos, p_unitSubString) == true)
				spk::throwException(L"Malformatted JSON number: " + p_unitSubString);

			exponentPos = (exponentPos == std::wstring::npos) ? p_unitSubString.size() : exponentPos;

			if (p_unitSubString.substr(isNegative, exponentPos - isNegative).find_first_not_of(L".0123456789") != std::wstring::npos)
				spk::throwException(L"JSON number value is not Numerical: " + p_unitSubString);

			if (exponentPos != p_unitSubString.size())
				exponent = _extractExponent(p_unitSubString.substr(exponentPos + 1));

			try
			{
				(_resultWillBeDouble(decimalPos, p_unitSubString.size() != exponentPos,
					exponentPos, isNegative, exponent) == true) ?
					result = std::stod(p_unitSubString) : result = std::stol(p_unitSubString);
			}
			catch (const std::exception& e)
			{
				spk::throwException(L"Invalid numbers JSON value: " + p_unitSubString + L" too big (number overflow)");
			}

			if (exponentPos != p_unitSubString.size() && std::holds_alternative<long>(result) == true)
				result = _safePowerOfTen(std::get<long>(result), exponent, p_unitSubString);

			p_objectToFill.set((std::holds_alternative<double>(result) == true) ?
				std::get<double>(result) : std::get<long>(result));
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

		static void _loadContent(spk::JSON::Object& p_objectToFill, const std::wstring& p_content, size_t& p_index);

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
				spk::JSON::Object newObject;

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

		File::File()
		{
		}

		File::File(const std::filesystem::path& p_filePath)
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