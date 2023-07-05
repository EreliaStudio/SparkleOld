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

		static bool _resultWillBeDouble(const size_t& p_decimalPos, const size_t& p_exponentPos,
			bool p_isNegative, const long& p_exponent)
		{
			return (p_decimalPos != std::wstring::npos ||
				p_exponent > spk::numberLength(std::numeric_limits<long>::max()) ||
				p_exponentPos - p_isNegative > spk::numberLength(std::numeric_limits<long>::max()) ||
				p_exponentPos - p_isNegative + p_exponent <= 0);
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

		void File::_loadUnitNumbers(spk::JSON::Object& p_objectToFill,
			const std::wstring& p_unitSubString)
		{
			std::variant<long, double> result;

			bool isNegative(p_unitSubString[0] == L'-');

			size_t exponentPos(p_unitSubString.find_first_of(L"eE"));
			long exponent(0);

			size_t decimalPos(p_unitSubString.find_last_of(L"."));
			bool isFractional(decimalPos != std::wstring::npos);

			if (_isNumberMalformatted(isNegative, decimalPos, exponentPos, p_unitSubString) == true)
				spk::throwException(L"Malformatted JSON number: " + p_unitSubString);

			exponentPos = (exponentPos == std::wstring::npos) ? p_unitSubString.size() : exponentPos;

			if (p_unitSubString.substr(isNegative, exponentPos - isNegative).find_first_not_of(L".0123456789") != std::wstring::npos)
				spk::throwException(L"JSON number value is not Numerical: " + p_unitSubString);

			if (exponentPos != p_unitSubString.size())
				exponent = _extractExponent(p_unitSubString.substr(exponentPos + 1));

			try
			{
				result = (_resultWillBeDouble(decimalPos, exponentPos, isNegative, exponent) == true) ?
					std::stod(p_unitSubString) : std::stol(p_unitSubString);
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
			p_objectToFill.setAsObject();

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
			p_objectToFill.setAsArray();

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