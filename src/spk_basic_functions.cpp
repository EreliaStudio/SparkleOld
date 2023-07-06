#include "spk_basic_functions.hpp"
#include <filesystem>
#include <algorithm>
#include <codecvt>

namespace spk
{
	std::wstring methodName(const std::string& prettyFunction)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wide = converter.from_bytes(prettyFunction);
		return methodName(wide);
	}

	std::wstring className(const std::string& prettyFunction)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wide = converter.from_bytes(prettyFunction);
		return className(wide);
	}

	std::wstring methodName(const std::wstring& prettyFunction)
	{
		size_t end = prettyFunction.rfind(L"(");
		if (end == std::wstring::npos) return L"";
		size_t beginColon = prettyFunction.rfind(L"::", end);
		if (beginColon != std::wstring::npos) beginColon += 2;
		size_t beginSpace = prettyFunction.rfind(L" ", end);
		if (beginSpace != std::wstring::npos) beginSpace += 1;
		size_t begin = std::max(beginColon, beginSpace);
		std::wstring result = prettyFunction.substr(begin, end - begin);
		return (result + L"()");
	}

	std::wstring className(const std::wstring& prettyFunction)
	{
		size_t classEnd = prettyFunction.rfind(L"(");
		if (classEnd == std::wstring::npos) return L"No class";
		size_t beginColon = prettyFunction.rfind(L"::", classEnd);
		if (beginColon == std::wstring::npos) return L"No class";
		size_t beginSpace = prettyFunction.rfind(L" ", beginColon);
		if (beginSpace != std::wstring::npos) beginSpace += 1;
		size_t classBegin = std::max(beginColon, beginSpace);
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

	int positive_modulo(int p_integer, int p_dividor)
	{
		int result = p_integer % p_dividor;
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

	void throwException(const std::wstring& p_errorLine) noexcept(false)
	{
		throw std::runtime_error(spk::wstringToString(p_errorLine).c_str());
	}
}