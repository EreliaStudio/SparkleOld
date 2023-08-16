#include "spk_basic_functions.hpp"
#include <codecvt>
#include <locale>

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

	int getLastSocketErrorValue()
	{
		return (strerror(errno));
	}

	void throwException(const std::wstring& p_errorLine) noexcept(false)
	{
		throw std::runtime_error(spk::wstringToString(p_errorLine).c_str());
	}

	void printCallStack()
	{
		
	}
}