#include "miscellaneous/JSON/spk_JSON_file.hpp"

#include <fstream>
#include <algorithm>
#include <cwctype>

#include "spk_basic_functions.hpp"

namespace spk
{
	namespace JSON
	{
		void File::_removeUnnecessaryChar(std::wstring& p_fileContent)
		{
			std::wstring result;
			bool isInQuotes = false;
			bool isSkipped = false;

			for (wchar_t c : p_fileContent)
			{
				switch (c)
				{
				case '\\':
					isSkipped = true;
					result += c;
					break;
				case '\"':
					if (isSkipped == false)
					{
						isInQuotes = !isInQuotes;
					}
					result += c;
					break;

				case '\t':
				case '\r':
				case '\n':
				case ' ':
					if (isInQuotes == false)
						break;
				default:
					result += c;
					isSkipped = false;
				}
			}

			p_fileContent = result;
		}

		std::wstring File::_loadFileContent(const std::filesystem::path& p_filePath)
		{
			std::wifstream wif(p_filePath);
			if (!wif.is_open())
			{
				throw std::runtime_error("Failed to open file");
			}

			std::wstringstream wss;
			wss << wif.rdbuf();
			std::wstring result = wss.str();

			_removeUnnecessaryChar(result);
			return (result);
		}

		std::wstring File::_removeEscapingChar(const std::wstring& p_string)
		{
			std::wstring result;
			bool previousWasBackslash = false;

			for (wchar_t c : p_string)
			{
				if (c == L'\\' && !previousWasBackslash)
				{
					previousWasBackslash = true;
				}
				else
				{
					result += c;
					previousWasBackslash = false;
				}
			}

			return result;
		}

		std::wstring File::_getAttributeName(const std::wstring& p_content, size_t& p_index)
		{
			if (p_content[p_index] != L'\"')
				spk::throwException(L"Invalid attribute name [" + p_content.substr(p_index) + L"]");

			p_index++;
			size_t start = p_index;
			bool isInsideQuote = true;
			for (; p_index < p_content.size() && (p_content[p_index] != ':' || isInsideQuote == true); p_index++)
			{
				if (p_content[p_index] == '\"' && p_content[p_index - 1] != '\\')
					isInsideQuote = !isInsideQuote;
			}
			p_index++; // Skipping the :
			return (_removeEscapingChar(p_content.substr(start, p_index - start - 2)));
		}

		std::wstring File::_extractUnitSubstring(const std::wstring& p_content, size_t& p_index)
		{
			size_t oldIndex = p_index;
			bool isInsideQuote = false;
			bool isSkipped = false;

			for (; p_index < p_content.size() &&
				((p_content[p_index] != ',' && p_content[p_index] != '}' && p_content[p_index] != ']') || isInsideQuote == true);
				p_index++)
			{
				if (p_content[p_index] == '\"' && p_content[p_index - 1] != '\\')
					isInsideQuote = !isInsideQuote;
			}

			return (_removeEscapingChar(p_content.substr(oldIndex, p_index - oldIndex)));
		}

		void File::_loadUnitString(spk::JSON::Object& p_objectToFill, const std::wstring& p_unitSubString)
		{
			p_objectToFill.set(p_unitSubString.substr(1, p_unitSubString.size() - 2));
		}

		/**
		 * @brief Assure that the string is a valid JSON's RFC integer
		 * @details number = [ minus ] int [ frac ] [ exp ]
		 *
		 * @param p_objectToFill The object that will receive the value
		 * @param p_unitSubString The substring that should only contain the number at this point.
		 * @throw spk::Exception if the string is not a valid JSON's RFC integer
		 */
		void File::_loadUnitNumbers(spk::JSON::Object& p_objectToFill,
			const std::wstring& p_unitSubString)
		{
			bool isNegative = p_unitSubString[0] == L'-';
			std::wstring digitsPart;
			std::variant<long, double> result;
			size_t exponentPos(0);
			bool isExponentSigned(false);
			bool isExponentNegative(false);
			long exponent(0);
			size_t decimalPos = p_unitSubString.find_last_of(L".");

			if (decimalPos == p_unitSubString.size() - 1 ||
				(decimalPos != std::wstring::npos &&
					::isdigit(p_unitSubString[decimalPos + 1]) == false))
				spk::throwException(L"Invalid fractional numbers JSON value: " + p_unitSubString);
			exponentPos = p_unitSubString.find(L"e");
			if (exponentPos == std::wstring::npos)
				exponentPos = p_unitSubString.find(L"E");
			if (decimalPos != std::wstring::npos &&
				exponentPos != std::wstring::npos &&
				decimalPos > exponentPos)
				spk::throwException(L"Invalid fractional numbers JSON value: " + p_unitSubString);
			if (exponentPos == std::wstring::npos)
				exponentPos = p_unitSubString.size();
			if ((isNegative == true && p_unitSubString.size() == 1) ||
				exponentPos == p_unitSubString.size() - 1 ||
				std::count(p_unitSubString.begin() + isNegative,
					p_unitSubString.begin() + exponentPos + (decimalPos != std::wstring::npos),
					'.') > 1 ||
				std::none_of(p_unitSubString.begin() + isNegative,
					p_unitSubString.begin() + exponentPos + (decimalPos != std::wstring::npos),
					[](wchar_t c) { return std::isdigit(c) || c == L'.'; }) ||
				(p_unitSubString[isNegative] == L'0' && p_unitSubString.size() > isNegative + 1 &&
					std::wstring(L".eE").find(p_unitSubString[isNegative + 1]) == std::wstring::npos))
				spk::throwException(L"Invalid numbers JSON value: " + p_unitSubString);
			digitsPart = p_unitSubString.substr(0, exponentPos);
			if (exponentPos != p_unitSubString.size())
			{
				isExponentSigned = p_unitSubString[exponentPos + 1] == L'-' ||
					p_unitSubString[exponentPos + 1] == L'+';
				isExponentNegative = p_unitSubString[exponentPos + 1] == L'-';
				exponentPos++;
				if (std::all_of(
					p_unitSubString.begin() + exponentPos + isExponentSigned,
					p_unitSubString.end(), ::isdigit) == false)
					spk::throwException(L"Invalid numbers JSON exponent value: " + p_unitSubString);
				try
				{
					exponent = std::stol(p_unitSubString.substr(exponentPos));
				}
				catch (const std::exception& e)
				{
					spk::throwException(L"Invalid numbers JSON value: " + p_unitSubString + L" too big (number overflow)");
				}
			}
			try
			{
				if (decimalPos != std::wstring::npos || exponent >
					spk::numberLength(std::numeric_limits<long>::max()) ||
					digitsPart.size() - isNegative >
					spk::numberLength(std::numeric_limits<long>::max()) ||
					digitsPart.size() - isNegative + exponent <= 0)
					result = std::stod(p_unitSubString);
				else
					result = std::stol(p_unitSubString);
			}
			catch (const std::exception& e)
			{
				spk::throwException(L"Invalid numbers JSON value: " +
					p_unitSubString + L" too big (number overflow)");
			}
			if (exponentPos != p_unitSubString.size())
			{
				errno = 0;
				std::feclearexcept(FE_ALL_EXCEPT);
				long* longValue = std::get_if<long>(&result);

				if (longValue != nullptr)
					*longValue *= std::pow(10, exponent);
				if (errno == EDOM || errno == ERANGE ||
					std::fetestexcept(FE_ALL_EXCEPT ^ FE_INEXACT) != 0)
					spk::throwException(L"Invalid numbers JSON value: " + p_unitSubString + L" too big (power overflow)");
			}
			p_objectToFill.set((std::get_if<double>(&result) != nullptr) ?
				std::get<double>(result) : std::get<long>(result));
		}

		void File::_loadUnitBoolean(spk::JSON::Object& p_objectToFill, const std::wstring& p_unitSubString)
		{
			if (p_unitSubString == L"true")
				p_objectToFill.set(true);
			else if (p_unitSubString == L"false")
				p_objectToFill.set(false);
			else
				spk::throwException(L"Invalid boolean JSON value: " + p_unitSubString);
		}

		void File::_loadUnitNull(spk::JSON::Object& p_objectToFill, const std::wstring& p_unitSubString)
		{
			if (p_unitSubString == L"null")
				p_objectToFill.set(nullptr);
			else
				spk::throwException(L"Invalid null JSON value: " + p_unitSubString);
		}

		void File::_loadUnit(spk::JSON::Object& p_objectToFill, const std::wstring& p_content, size_t& p_index)
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

		void File::_loadObject(spk::JSON::Object& p_objectToFill, const std::wstring& p_content, size_t& p_index)
		{
			p_index++; // Skip the {
			for (; p_index < p_content.size() && p_content[p_index] != '}';)
			{
				std::wstring attributeName = _getAttributeName(p_content, p_index);

				spk::JSON::Object& newObject = p_objectToFill.addAttribute(attributeName);

				_loadContent(newObject, p_content, p_index);

				if (p_content[p_index] == ',')
					p_index++;
			}
			p_index++; // Skip the }
		}

		void File::_loadArray(spk::JSON::Object& p_objectToFill, const std::wstring& p_content, size_t& p_index)
		{
			p_index++;
			for (; p_index < p_content.size() && p_content[p_index] != ']';)
			{
				spk::JSON::Object newObject;

				_loadContent(newObject, p_content, p_index);

				p_objectToFill.push_back(newObject);

				if (p_content[p_index] == ',')
					p_index++;
			}
			p_index++;
		}

		void File::_loadContent(spk::JSON::Object& p_objectToFill, const std::wstring& p_content, size_t& p_index)
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
			_root.reset();
			_loadContent(_root, fileContent, index);
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