#pragma once

#include <string>
#include <vector>
#include <fstream>
#include "iostream/spk_iostream.hpp"

namespace spk
{
	/**
	 * @brief Extracts the method name from the provided pretty function.
	 * @param prettyFunction The pretty function string.
	 * @return The extracted method name.
	 */
	std::wstring methodName(const std::string& prettyFunction);

	/**
	 * @brief Extracts the class name from the provided pretty function.
	 * @param prettyFunction The pretty function string.
	 * @return The extracted class name.
	 */
	std::wstring className(const std::string& prettyFunction);

	/**
	 * @brief Extracts the method name from the provided wide string pretty function.
	 * @param prettyFunction The wide string pretty function.
	 * @return The extracted method name.
	 */
	std::wstring methodName(const std::wstring& prettyFunction);

	/**
	 * @brief Extracts the class name from the provided wide string pretty function.
	 * @param prettyFunction The wide string pretty function.
	 * @return The extracted class name.
	 */
	std::wstring className(const std::wstring& prettyFunction);

	/**
	 * @brief Splits the input wide string into a vector of substrings based on the provided delimiter.
	 * @param p_string The input wide string to split.
	 * @param p_delim The delimiter to split the string.
	 * @return A vector of substrings after splitting.
	 */
	std::vector<std::wstring> stringSplit(const std::wstring& p_string, const std::wstring& p_delim);

	/**
	 * @brief Reads a wide character from the provided wide file stream.
	 * @param p_file The wide file stream to read from.
	 * @return The readed wide character.
	 */
	wchar_t getChar(std::wfstream& p_file);

	/**
	 * @brief Reads a wide string from the provided wide file stream.
	 * @param p_file The wide file stream to read from.
	 * @return The readed wide string.
	 */
	std::wstring getStr(std::wfstream& p_file);

	/**
	 * @brief Reads and splits a wide string from the provided wide file stream based on the delimiter.
	 * @param p_file The wide file stream to read from.
	 * @param p_delim The delimiter to split the string.
	 * @param p_expectedSize The expected size of the resulting vector. Default is -1, indicating no specific size.
	 * @return A vector of substrings after splitting.
	 */
	std::vector<std::wstring> getStringSplit(std::wfstream& p_file, const std::wstring& p_delim, const int& p_expectedSize = -1);

	/**
	 * @brief Calculates the positive modulo of the given integer and divisor.
	 * @param p_integer The integer value.
	 * @param p_dividor The divisor.
	 * @return The positive modulo result.
	 */
	int positive_modulo(int p_integer, int p_dividor);

	/**
	 * @brief Lists all files in the specified directory path with the provided extension.
	 * @param p_path The directory path.
	 * @param p_extension The file extension to filter files.
	 *                    Default is "*" (matches all files).
	 * @return A vector of file names.
	 */
	std::vector<std::wstring> listFile(const std::wstring& p_path, const std::wstring& p_extension = L"*");

	/**
	 * @brief Lists all directories in the specified directory path.
	 * @param p_path The directory path.
	 * @return A vector of directory names.
	 */
	std::vector<std::wstring> listDir(const std::wstring& p_path);

	/**
	 * @brief Converts a wide string to a standard string.
	 * @param p_wstr The wide string to convert.
	 * @return The converted standard string.
	 */
	std::string wstringToString(const std::wstring& p_wstr);

	/**
	 * @brief Throws a runtime_error with the provided error message as wide string.
	 * @param p_errorLine The error message.
	 */
	void throwException(const std::wstring& p_errorLine);

	/**
	 * @brief Calculates the length of a numeric value in a specified base.
	 * @tparam NumericType The type of the numeric value.
	 * @param p_number The numeric value.
	 * @param base The base in which to calculate the length. Default is 10.
	 * @return The length of the numeric value.
	 */
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
/**
 * @def DEBUG_LINE()
 * @brief Prints the current method name and line number for debugging purposes.
 */
#define DEBUG_LINE() spk::cout << __METHOD__ << "::" << __LINE__ << std::endl
#endif