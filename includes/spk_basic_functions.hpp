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
	
	std::vector<std::wstring> listDir(const std::wstring& p_path);
}

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
	#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#ifndef __METHOD__
	#define __METHOD__ spk::methodName(__PRETTY_FUNCTION__)
#endif

#ifndef __CLASS__ 
	#define __CLASS__ spk::className(__PRETTY_FUNCTION__)
#endif