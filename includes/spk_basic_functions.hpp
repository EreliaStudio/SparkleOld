#pragma once

#include <string>
#include <vector>
#include <fstream>

namespace spk
{
	std::wstring methodName(const std::wstring& prettyFunction);
	std::wstring className(const std::wstring& prettyFunction);

	std::vector<std::wstring> stringSplit(const std::wstring& p_string, const std::wstring& p_delim);

	std::wstring getStr(std::wfstream& p_file);
	std::vector<std::wstring> getStringSplit(std::wfstream& p_file, const std::wstring& p_delim, const int& p_expectedSize = -1);

	int positive_modulo(int i, int n);
	std::vector<std::wstring> listFile(const std::wstring& p_path, const std::wstring& p_extension);
}