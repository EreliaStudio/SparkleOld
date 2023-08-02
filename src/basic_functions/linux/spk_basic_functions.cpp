#include "spk_basic_functions.hpp"
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

	void printCallStack()
	{
		
	}
}