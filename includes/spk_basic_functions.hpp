#pragma once

#include <string>
#include <vector>
#include <fstream>
#include "iostream/spk_iostream.hpp"
#include <stdexcept>
#include "spk_system_define.hpp"

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
	intmax_t positiveModulo(intmax_t p_integer, intmax_t p_dividor);

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
	 * @brief Redirect an exception emitted by a thread during the execution of a job.
	 * @param e The orginal exception.
	 * @param p_jobName The name of the job who just thrown an exception.
	*/
	void redirectException(std::exception& e, const std::wstring* p_jobName);

	/**
	 * @brief Turns a non printable or unicode character into its universal code.
	 * 
	 * @param p_code A non printable or unicode character. ]0-31[ or ]127-0x10FFFF[
	 * @return std::wstring A string containing the universal code of the character.
	 * (\\uXXXX or \\uXXXX\\uXXXX respectively for 8 or 16 bit characters)
	 * @note If the character is in range ]32-126[, it is returned as is.
	 * 	If the character code is higher than 0x10FFFF, an empty string is returned.
	 */
	std::wstring universalCodeDecoder(const wchar_t& p_code);

	/**
	 * @brief Turns a universal code into a non printable or unicode character.
	 * 
	 * @param p_code The wstring containing the universal code of the character.
	 * @return wchar_t A non printable or unicode character. ]0-31[ or ]127-0x10FFFF[
	 * @warning If your local does not support UTF16,
	 * you should use try/catch if you plan to convert or print the result.
	 */
	wchar_t universalCodeEncoder(const std::wstring& p_code);

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

	/**
	 * @brief Print the callstack of the current program.
	 */
	void printCallStack();

	/**
	 * @brief Convert a string to a wstring
	*/
	std::wstring stringToWString(const std::string &s);
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
#define DEBUG_LINE() spk::cout << __CLASS__ << "::" << __METHOD__ << " - line: " << __LINE__ << std::endl
#endif
