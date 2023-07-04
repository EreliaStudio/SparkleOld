#pragma once

#include <string>
#include <vector>
#include <fstream>
#include "iostream/spk_iostream.hpp"

namespace spk
{
	std::wstring methodName(const std::string& prettyFunction);
	std::wstring className(const std::string& prettyFunction);
	std::wstring methodName(const std::wstring& prettyFunction);
	std::wstring className(const std::wstring& prettyFunction);

	std::vector<std::wstring> stringSplit(const std::wstring& p_string, const std::wstring& p_delim);

	wchar_t getChar(std::wfstream& p_file);
	std::wstring getStr(std::wfstream& p_file);
	std::vector<std::wstring> getStringSplit(std::wfstream& p_file, const std::wstring& p_delim, const int& p_expectedSize = -1);

	int positive_modulo(int i, int n);
	std::vector<std::wstring> listFile(const std::wstring& p_path, const std::wstring& p_extension);

	std::vector<std::wstring> listDir(const std::wstring& p_path);

	std::string wstringToString(const std::wstring& p_wstr);

	void throwException(const std::wstring& p_errorLine);

	template <typename NumericType>
	unsigned short numberLength(NumericType p_number, unsigned short base = 10)
	{
		unsigned short result(1);

		if (base == 0)
			throw std::runtime_error("numberLength: Base cannot be 0");
		if (p_number < 0)
		{
			++result;
			p_number = -p_number;
		}
		while (p_number >= base)
		{
			p_number /= base;
			++result;
		}
		return (result);
	}
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

#ifdef DEBUG_LINE
#undef DEBUG_LINE
#define DEBUG_LINE() spk::cout << __METHOD__ << "::" << __LINE__ << std::endl
#endif