#include "miscellaneous/JSON/spk_JSON_file.hpp"

#include <fstream>
#include <algorithm>
#include <cwctype>

#include "spk_basic_functions.hpp"

namespace spk
{
	namespace JSON
	{

		/**
		 * @brief Check if the given string is a correct JSON number.
		 *
		 * @param p_isNegative Indicates if the number is negative.
		 * @param p_decimalPos Location of the decimal point. (std::wstring::npos if there is no decimal point)
		 * @param p_exponentPos Location of the exponent. (std::wstring::npos if there is no exponent)
		 * @param p_unitSubString The string to check.
		 * @return True if the string is not a correct JSON number, false otherwise.
		 */
		static bool _isNumberMalformatted(bool p_isNegative, const size_t& p_decimalPos,
			const size_t& p_exponentPos, const std::wstring& p_unitSubString)
		{
			return (p_exponentPos == p_unitSubString.size() - 1 ||
				(p_isNegative == true && p_unitSubString.size() == 1) ||
				(p_isNegative == true && p_unitSubString.size() > 1 && ::isdigit(p_unitSubString[1]) == false) ||
				p_decimalPos == p_unitSubString.size() - 1 ||
				(p_decimalPos != std::wstring::npos && ::isdigit(p_unitSubString[p_decimalPos + 1]) == false) ||
				(p_decimalPos != std::wstring::npos && p_exponentPos != std::wstring::npos && p_decimalPos > p_exponentPos) ||
				(p_unitSubString[p_isNegative] == L'0' && p_unitSubString.size() > static_cast<size_t>(p_isNegative) + 1u &&
					std::wstring(L".eE").find(p_unitSubString[p_isNegative + 1]) == std::wstring::npos) ||
				std::count(p_unitSubString.begin(), p_unitSubString.end(), L'.') > 1);
		}

		/**
		 * @brief Extracts the number's exponent from the string.
		 *
		 * @param p_exponentSubstring The string containing the number's exponent.
		 * @return long The number's exponent.
		 */
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
			catch (const std::exception&)
			{
				spk::throwException(L"Invalid numbers JSON value: " + p_exponentSubstring + L" too big (number overflow)");
			}
			return (result);
		}

		/**
		 * @brief Check if the string parsed so far will be a real number.
		 *
		 * @param p_decimalPos The position of the decimal point. (std::wstring::npos if there is no decimal point)
		 * @param p_hasExponent Information about the number's exponent.
		 * @param p_exponentPos The position of the exponent.
		 * @param p_isNegative Is the number negative?
		 * @param p_exponent The number's exponent.
		 * @return True If the number will be a real number. False otherwise.
		 */
		static bool _resultWillBeDouble(const size_t& p_decimalPos, bool p_hasExponent, const size_t& p_exponentPos,
			bool p_isNegative, const long& p_exponent)
		{
			if (p_decimalPos == std::wstring::npos && p_exponentPos == false)
				return false;
			return (p_decimalPos != std::wstring::npos || (p_hasExponent == true &&
				p_exponent > spk::numberLength(std::numeric_limits<long>::max()) ||
				p_exponentPos - p_isNegative > spk::numberLength(std::numeric_limits<long>::max()) ||
				p_exponentPos - p_isNegative + p_exponent <= 0));
		}

		/**
		 * @brief Safely computes the power of ten of the specified number.
		 * Checks for any computation error and throws an exception if any.
		 *
		 * @param p_number The number to compute the power of ten of.
		 * @param p_exponent The exponent to use.
		 * @param p_unitSubString The stringifyed number used in the exception message if any.
		 * @return long The result of the computation.
		 */
		static long _safePowerOfTen(const long& p_number, const long& p_exponent, const std::wstring& p_unitSubString)
		{
			long result(0);
			errno = 0;
			std::feclearexcept(FE_ALL_EXCEPT);

			result = static_cast<long>(p_number * std::pow(10, p_exponent));

			if (errno == EDOM || errno == ERANGE || std::fetestexcept(FE_ALL_EXCEPT ^ FE_INEXACT) != 0)
				spk::throwException(L"Invalid numbers JSON value: " + p_unitSubString + L" too big (power overflow)");

			return (result);
		}

		/**
		 * @brief Loads the unit numbers into the specified JSON object.
		 * @param p_objectToFill The JSON object to fill with the unit numbers.
		 * @param p_unitSubString The unit substring to load.
		 */
		void _loadUnitNumbers(spk::JSON::Object& p_objectToFill, const std::wstring& p_unitSubString)
		{
			std::variant<long, double> result;

			bool isNegative(p_unitSubString[0] == L'-');

			size_t exponentPos(p_unitSubString.find_first_of(L"eE"));
			long exponent(0);

			size_t decimalPos(p_unitSubString.find_last_of(L"."));

			if (_isNumberMalformatted(isNegative, decimalPos, exponentPos, p_unitSubString) == true)
				spk::throwException(L"Malformatted JSON number: " + p_unitSubString);

			exponentPos = (exponentPos == std::wstring::npos) ? p_unitSubString.size() : exponentPos;

			if (p_unitSubString.substr(isNegative, exponentPos - isNegative).find_first_not_of(L".0123456789") != std::wstring::npos)
				spk::throwException(L"JSON number value is not Numerical: " + p_unitSubString);

			if (exponentPos != p_unitSubString.size())
				exponent = _extractExponent(p_unitSubString.substr(exponentPos + 1));

			try
			{
				(_resultWillBeDouble(decimalPos, p_unitSubString.size() != exponentPos,
					exponentPos, isNegative, exponent) == true) ?
					result = std::stod(p_unitSubString) : result = std::stol(p_unitSubString);
			}
			catch (const std::exception&)
			{
				spk::throwException(L"Invalid numbers JSON value: " + p_unitSubString + L" too big (number overflow)");
			}

			if (exponentPos != p_unitSubString.size() && std::holds_alternative<long>(result) == true)
				result = _safePowerOfTen(std::get<long>(result), exponent, p_unitSubString);

			p_objectToFill.set((std::holds_alternative<double>(result) == true) ?
				std::get<double>(result) : std::get<long>(result));
		}
	}
}