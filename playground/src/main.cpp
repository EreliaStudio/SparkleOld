#include "playground.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <codecvt>

class File
{
private:
	spk::JSON::Object _root;

	void _removeUnnecessaryChar(std::wstring& p_fileContent)
	{
		std::wstring result;
		bool isInQuotes = false;
		bool isSkipped = false;

		for (wchar_t c : p_fileContent) {
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

	std::wstring _loadFileContent(const std::wstring& p_filePath)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::string narrowFilePath = converter.to_bytes(p_filePath);
		
		std::wifstream wif(narrowFilePath);
		if (!wif.is_open())
		{
			throw std::runtime_error("Failed to open file");
		}
		std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
		wif.imbue(loc);
		std::wstringstream wss;
		wss << wif.rdbuf();
		std::wstring result = wss.str();

		_removeUnnecessaryChar(result);
		return (result);
	}

	std::wstring _removeEscapingChar(const std::wstring& p_string)
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

	std::wstring _getAttributeName(const std::wstring& p_content, size_t& p_index)
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
		p_index++;//Skipping the :
		return (_removeEscapingChar(p_content.substr(start, p_index - start - 2)));
	}

	static const int _ERROR_TYPE = -1;
	static const int _UNIT_TYPE = 0;
	static const int _OBJECT_TYPE = 1;
	static const int _ARRAY_TYPE = 2;

	int _getLoadingType(const std::wstring& p_content, size_t& p_index)
	{
		int result = _ERROR_TYPE;

		switch (p_content[p_index])
		{
			case '{':
				result = (_OBJECT_TYPE);
				break;
			case '[':
				result = (_ARRAY_TYPE);
				break;
			default :
				result = (_UNIT_TYPE);
				break;
		}

		return (result);
	}

	std::wstring _extractUnitSubstring(const std::wstring& p_content, size_t& p_index)
	{
		size_t oldIndex = p_index;
		bool isInsideQuote = false;
		bool isSkipped = false;
		
		for (; p_index < p_content.size() &&
			((p_content[p_index] != ',' && p_content[p_index] != '}' && p_content[p_index] != ']')  || isInsideQuote == true); p_index++)
		{
			if (p_content[p_index] == '\"' && p_content[p_index - 1] != '\\')
				isInsideQuote = !isInsideQuote;
		}

		return (_removeEscapingChar(p_content.substr(oldIndex, p_index - oldIndex)));
	}

	static const int _ERROR_UNIT_TYPE = -1;
	static const int _STRING_UNIT_TYPE = 0;
	static const int _INT_UNIT_TYPE = 1;
	static const int _DOUBLE_UNIT_TYPE = 2;
	static const int _BOOL_UNIT_TYPE = 3;
	static const int _NULL_UNIT_TYPE = 4;

	int _getUnitType(const std::wstring& p_unitSubString)
	{
		switch (p_unitSubString[0])
		{
			case '\"':
				return (_STRING_UNIT_TYPE);
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			{
				if (p_unitSubString.find(L'.') != std::wstring::npos)
					return (_DOUBLE_UNIT_TYPE);
				else
					return (_INT_UNIT_TYPE);
			}
			case 'N':
			case 'n':
				return (_NULL_UNIT_TYPE);
			case 't':
			case 'T':
			case 'f':
			case 'F':
				return (_BOOL_UNIT_TYPE);
		}
		return (_ERROR_UNIT_TYPE);
	}

	void _loadUnitString(spk::JSON::Object& p_objectToFill, const std::wstring& p_unitSubString)
	{
		p_objectToFill.set(p_unitSubString.substr(1, p_unitSubString.size() - 2));
	}

	void _loadUnitInt(spk::JSON::Object& p_objectToFill, const std::wstring& p_unitSubString)
	{
		if (!std::all_of(p_unitSubString.begin(), p_unitSubString.end(), ::isdigit))
		{
			spk::throwException(L"Invalid integer JSON value: " + p_unitSubString);
		}

        p_objectToFill.set(std::stoi(p_unitSubString));
	}

	void _loadUnitDouble(spk::JSON::Object& p_objectToFill, const std::wstring& p_unitSubString)
	{
		if (std::count(p_unitSubString.begin(), p_unitSubString.end(), '.') > 1 ||
			std::none_of(p_unitSubString.begin(), p_unitSubString.end(), [](char c) { return std::isdigit(c) || c == '.'; })) 
		{
			spk::throwException(L"Invalid double JSON value: " + p_unitSubString);
		}

		p_objectToFill.set(std::stod(p_unitSubString));
	}

	void _loadUnitBoolean(spk::JSON::Object& p_objectToFill, const std::wstring& p_unitSubString)
	{
		std::wstring tmp = p_unitSubString;
		std::transform(tmp.begin(), tmp.end(), tmp.begin(),
                   [](wchar_t c){ return std::towlower(c); });
		if (tmp == L"true")
			p_objectToFill.set(true);
		else if (tmp == L"false")
			p_objectToFill.set(false);
		else
			spk::throwException(L"Invalid boolean JSON value: " + p_unitSubString);
	}

	void _loadUnitNull(spk::JSON::Object& p_objectToFill, const std::wstring& p_unitSubString)
	{
		std::wstring tmp = p_unitSubString;
		std::transform(tmp.begin(), tmp.end(), tmp.begin(),
                   [](wchar_t c){ return std::towlower(c); });

		if (tmp == L"null" || tmp == L"nullptr")
			p_objectToFill.set(nullptr);
		else
			spk::throwException(L"Invalid null JSON value: " + p_unitSubString);
	}

	void _loadUnit(spk::JSON::Object& p_objectToFill, const std::wstring& p_content, size_t& p_index)
	{
		std::wstring substring = _extractUnitSubstring(p_content, p_index);
		switch (_getUnitType(substring))
		{
			case _STRING_UNIT_TYPE:
				_loadUnitString(p_objectToFill, substring);
				break;
			case _INT_UNIT_TYPE:
				_loadUnitInt(p_objectToFill, substring);
				break;
			case _DOUBLE_UNIT_TYPE:
				_loadUnitDouble(p_objectToFill, substring);
				break;
			case _BOOL_UNIT_TYPE:
				_loadUnitBoolean(p_objectToFill, substring);
				break;
			case _NULL_UNIT_TYPE:
				_loadUnitNull(p_objectToFill, substring);
				break;
		}
	}

	void _loadObject(spk::JSON::Object& p_objectToFill, const std::wstring& p_content, size_t& p_index)
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

	void _loadArray(spk::JSON::Object& p_objectToFill, const std::wstring& p_content, size_t& p_index)
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

	void _loadContent(spk::JSON::Object& p_objectToFill, const std::wstring& p_content, size_t& p_index)
	{
		int dataType = _getLoadingType(p_content, p_index);

		switch (dataType)
		{
			case _UNIT_TYPE:
				_loadUnit(p_objectToFill, p_content, p_index);
				break;
			case _OBJECT_TYPE:
				_loadObject(p_objectToFill, p_content, p_index);
				break;
			case _ARRAY_TYPE:
				_loadArray(p_objectToFill, p_content, p_index);
				break;
			default:
				throw std::runtime_error("Unexpected data type in JSON");
		}
	}

public:
	File()
	{

	}

	File(const std::wstring& p_filePath)
	{
		load(p_filePath);
	}

	void load(const std::wstring& p_filePath)
	{
		std::wstring fileContent = _loadFileContent(p_filePath);

		size_t index = 0;
		_loadContent(_root, fileContent, index);
	}

	const spk::JSON::Object& operator [] (const std::wstring& p_key) const
	{
		return (_root[p_key]);
	}

	const spk::JSON::Object& operator[](size_t p_index) const
	{
		return (_root[p_index]);
	}

	template<typename TType>
	const TType& as()
	{
		return (_root.as<TType>());
	}

	const spk::JSON::Object& root() const
	{
		return (_root);
	}

	friend std::wostream& operator << (std::wostream& p_os, const File& p_file)
	{
		p_os << p_file._root;
		return (p_os);
	}
};




int main()
{
	File file(L"test_json.json");

	spk::cout << "File : " << std::endl << file << std::endl;

	return (0);
}