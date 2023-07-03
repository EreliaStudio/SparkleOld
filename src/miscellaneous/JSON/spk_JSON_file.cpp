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
		void File::_loadUnitInt(spk::JSON::Object& p_objectToFill,
			const std::wstring& p_unitSubString)
		{
			bool isNegative = p_unitSubString[0] == L'-';
			long longResult(0);
			double doubleResult(0);
			size_t exponentPos(0);
			bool isExponentSigned(false);
			bool isExponentNegative(false);
			long exponent(0);

			exponentPos = p_unitSubString.find(L"e");
			if (exponentPos == std::wstring::npos)
				exponentPos = p_unitSubString.find(L"E");
			if (exponentPos == std::wstring::npos)
				exponentPos = p_unitSubString.size();
			if ((isNegative == true && p_unitSubString.size() == 1) ||
				exponentPos == p_unitSubString.size() - 1 ||
				std::all_of(p_unitSubString.begin() + isNegative,
					p_unitSubString.begin() + exponentPos, ::isdigit) == false ||
				p_unitSubString[isNegative] == L'0')
				spk::throwException(L"Invalid integer JSON value: " + p_unitSubString);
			try
			{
				longResult = std::stol(p_unitSubString, &exponentPos);
			}
			catch (const std::exception& e)
			{
				spk::throwException(L"Invalid integer JSON value: " + p_unitSubString + L" too big (long overflow)");
			}
			if (exponentPos != p_unitSubString.size())
			{
				isExponentSigned = p_unitSubString[exponentPos + 1] == L'-' ||
					p_unitSubString[exponentPos + 1] == L'+';
				isExponentNegative = p_unitSubString[exponentPos + 1] == L'-';
				exponentPos++;
				if (std::all_of(
					p_unitSubString.begin() + exponentPos + isExponentSigned,
					p_unitSubString.end(),
					::isdigit) == false)
					spk::throwException(L"Invalid integer JSON value: " + p_unitSubString);
				try
				{
					exponent = std::stol(p_unitSubString.substr(exponentPos));
				}
				catch (const std::exception& e)
				{
					spk::throwException(L"Invalid integer JSON value: " + p_unitSubString + L" too big (long overflow)");
				}
				errno = 0;
				std::feclearexcept(FE_ALL_EXCEPT);
				double power = std::pow(10, exponent);
				
				if (isExponentNegative == true)
					doubleResult = longResult * power;
				else
					longResult *= power;
				if (errno == EDOM || errno == ERANGE ||
					std::fetestexcept(FE_ALL_EXCEPT ^ FE_INEXACT) != 0)
					spk::throwException(L"Invalid integer JSON value: " + p_unitSubString + L" too big (double overflow)");
			}

			if (isExponentNegative == true)
				p_objectToFill.set(doubleResult);
			else
				p_objectToFill.set(longResult);
		}

		void File::_loadUnitDouble(spk::JSON::Object& p_objectToFill, const std::wstring& p_unitSubString)
		{
			if (std::count(p_unitSubString.begin(), p_unitSubString.end(), '.') > 1 ||
				std::none_of(p_unitSubString.begin(), p_unitSubString.end(), [](wchar_t c)
					{ return std::isdigit(c) || c == L'.'; }))
			{
				spk::throwException(L"Invalid double JSON value: " + p_unitSubString);
			}

			p_objectToFill.set(std::stod(p_unitSubString));
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
				if (substring.find(L'.') != std::wstring::npos)
					_loadUnitDouble(p_objectToFill, substring);
				else
					_loadUnitInt(p_objectToFill, substring);
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
				throw std::runtime_error(wstringToString(std::wstring(L"Unexpected data type in JSON: ") + p_content.substr(p_index, 10)));
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