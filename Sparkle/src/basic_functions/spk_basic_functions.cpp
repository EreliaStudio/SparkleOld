#include "spk_basic_functions.hpp"
#include <filesystem>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace spk
{
	std::wstring methodName(const std::wstring& prettyFunction)
	{
		size_t end = prettyFunction.rfind(L"(");
		if (end == std::wstring::npos)
			return L"";

		size_t beginSpace = prettyFunction.rfind(L" ", end);
		if (beginSpace == std::wstring::npos)
			beginSpace = 0;

		size_t beginColon = prettyFunction.rfind(L"::", end);
		if (beginColon != std::wstring::npos)
			beginColon += 2;
		else if (beginColon == std::wstring::npos)
			return (prettyFunction.substr(beginSpace, end - beginSpace));

		if (beginSpace != std::wstring::npos)
			beginSpace += 1;

		size_t begin = std::max<size_t>(beginColon, beginSpace);

		std::wstring result = prettyFunction.substr(begin, end - begin);
		return (result + L"()");
	}

	std::wstring className(const std::wstring& prettyFunction)
	{
		size_t classEnd = prettyFunction.rfind(L"(");
		if (classEnd == std::wstring::npos)
			return L"No class";

		size_t beginSpace = prettyFunction.rfind(L" ", classEnd);
		if (beginSpace != std::wstring::npos)
			beginSpace += 1;
		else if (beginSpace == std::wstring::npos)
			beginSpace = 0;

		size_t beginColon = prettyFunction.rfind(L"::", classEnd);
		if (beginColon == std::wstring::npos)
			return L"No class";
		if (beginColon == std::wstring::npos)
			return (prettyFunction.substr(beginSpace, classEnd - beginSpace));

		size_t classBegin = std::max<size_t>(beginColon, beginSpace);
		int closingBracket = 0;
		size_t resultStart = 0;

		for (int i = static_cast<int>(classBegin) - 1; i >= 0; i--)
		{
			if (prettyFunction[i] == L'>')
				closingBracket++;
			else if (prettyFunction[i] == L'<')
				closingBracket--;
			else if ((prettyFunction[i] == L' ' || prettyFunction[i] == L'\t') && closingBracket == 0)
				break;

			resultStart = i;
		}

		return (prettyFunction.substr(resultStart, classBegin - resultStart));
	}

	std::vector<std::wstring> stringSplit(const std::wstring& p_string, const std::wstring& p_delim)
	{
		std::vector<std::wstring> result;

		size_t start = 0;
		size_t end = p_string.find(p_delim);
		while (end != std::wstring::npos)
		{
			result.push_back(p_string.substr(start, end - start));
			start = end + static_cast<size_t>(p_delim.length());
			end = p_string.find(p_delim, start);
		}

		result.push_back(p_string.substr(start, end - start));

		return (result);
	}

	wchar_t getChar(std::wfstream& p_file)
	{
		wchar_t c;

		if (p_file.eof())
			return (L'\0');
		p_file.get(c);
		return (c);
	}

	std::wstring getStr(std::wfstream& p_file)
	{
		std::wstring line;

		if (p_file.eof())
			return (L"");
		std::getline(p_file, line);
		if (line.size() == 0)
			return (L"");
		if (line[line.size() - 1] == L'\n')
			line[line.size() - 1] = L'\0';
		return (line);
	}

	std::vector<std::wstring> getStringSplit(std::wfstream& p_file, const std::wstring& p_delim, const int& p_expectedSize)
	{
		std::wstring line = getStr(p_file);
		std::vector<std::wstring> result = stringSplit(line, p_delim);
		if (p_expectedSize != -1 && result.size() != p_expectedSize)
		{
			throw std::runtime_error("Unexpected number of string after parsing");
		}

		return (result);
	}

	std::wstring getFileContent(const std::filesystem::path& p_filePath)
	{
		std::wifstream shaderFile(p_filePath);
		if (shaderFile.is_open() == false) {
			spk::throwException(L"Failed to open file [" + p_filePath.wstring() + L"]");
		}

		std::wstringstream buffer;
		buffer << shaderFile.rdbuf();
		std::wstring result = buffer.str();
		return (result);
	}

	std::string getFileContentAsString(const std::filesystem::path& p_filePath)
	{
		std::ifstream shaderFile(p_filePath);
		if (shaderFile.is_open() == false) {
			spk::throwException(L"Failed to open file [" + p_filePath.wstring() + L"]");
		}

		std::stringstream buffer;
		buffer << shaderFile.rdbuf();
		std::string result = buffer.str();
		return (result);
	}

	intmax_t positiveModulo(intmax_t p_integer, intmax_t p_dividor)
	{
		intmax_t result = p_integer % p_dividor;
		while (result < 0)
			result += p_dividor;
		return result;
	}

	std::vector<std::wstring> listFile(const std::wstring& p_path, const std::wstring& p_extension)
	{
		std::vector<std::wstring> result;
		for (const auto& entry : std::filesystem::directory_iterator(p_path))
		{
			if (std::filesystem::is_regular_file(entry) && (p_extension == L"*" || entry.path().extension() == p_extension))
			{
				std::wstring tmpFile = entry.path().wstring();
				result.push_back(tmpFile);
			}
		}
		return result;
	}

	std::vector<std::wstring> listDir(const std::wstring& p_path)
	{
		std::vector<std::wstring> result;
		for (const auto& entry : std::filesystem::directory_iterator(p_path))
		{
			if (std::filesystem::is_directory(entry))
			{
				std::wstring tmpDir = entry.path().wstring();
				result.push_back(tmpDir);
			}
		}
		return result;
	}

	std::string wstringToString(const std::wstring& p_wstr)
	{
		std::string str(p_wstr.length(), ' ');
		std::transform(p_wstr.begin(), p_wstr.end(), str.begin(),
			[](wchar_t ch)
			{
				return ch <= 0xFF ? static_cast<char>(ch) : '?';
			});
		return str;
	}

	void redirectException(std::exception& e, const std::wstring* p_jobName)
	{
		spk::cout << "Unexpected throw";
		if (p_jobName != nullptr)
			spk::cout << " during execution of [" << *p_jobName << "] job";
		spk::cout << "\n";
		spk::cout << std::wstring(spk::cout.prefixSize() + 5, L' ') << "Error : [" << e.what() << "]" << std::endl;
	}

	std::wstring universalCodeDecoder(const wchar_t& p_code)
	{
		std::wstring result;
		std::wstringstream ss;

		if (isprint(p_code) == false || (p_code > 127 && p_code < 0xFFFF))
		{
			ss << L"\\u" << std::hex << std::setw(4) << std::setfill(L'0') << (int)p_code;
			result = ss.str();
		}
		else if (p_code > 0xFFFF && p_code < 0x10FFFF)
		{
			int highSurrogate((int)p_code - 0x10000);
			int lowSurrogate(highSurrogate & 0x3FF);

			highSurrogate = (highSurrogate >> 10) + 0xD800;
			lowSurrogate += 0xDC00;

			ss << L"\\u" << std::hex << std::setw(4) << std::setfill(L'0') << highSurrogate <<
				L"\\u" << std::hex << std::setw(4) << std::setfill(L'0') << lowSurrogate;
			result = ss.str();
		}
		else if (p_code > 0x10FFFF)
			result = L"";
		else
			result += p_code;
		return (result);
	}

	wchar_t universalCodeEncoder(const std::wstring& p_code)
	{
		wchar_t result = 0;

		if (p_code.size() == 6 && p_code[0] == L'\\' && p_code[1] == L'u')
		{
			result = static_cast<wchar_t>(std::stoi(p_code.substr(2, 4), nullptr, 16));
		}
		else if (p_code.size() == 12 && p_code[0] == L'\\' && p_code[1] == L'u' && p_code[6] == L'\\' && p_code[7] == L'u')
		{
			int highSurrogate = static_cast<int>(std::stoi(p_code.substr(2, 4), nullptr, 16));
			int lowSurrogate = static_cast<int>(std::stoi(p_code.substr(8, 4), nullptr, 16));

			highSurrogate = (highSurrogate - 0xD800) << 10;
			lowSurrogate -= 0xDC00;
			result = static_cast<wchar_t>(highSurrogate + lowSurrogate + 0x10000);
		}
		else
			result = p_code[0];
		return (result);
	}

	std::wstring to_wstring(const std::string& s)
	{
		std::wstring result(s.begin(), s.end());

		return (result);
	}

    float degreeToRadian(float degrees)
	{
        return static_cast<float>(degrees * (M_PI / 180.0f));
    }

    float radianToDegree(float radians)
	{
        return static_cast<float>(radians * (180.0f / M_PI));
    }
}