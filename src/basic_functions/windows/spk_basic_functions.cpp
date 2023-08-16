#include "spk_basic_functions.hpp"
#include "spk_system_define.hpp"

#include <Windows.h>
#include <dbghelp.h>

#pragma comment(lib, "dbghelp.lib")

namespace spk
{
	std::wstring methodName(const std::string& prettyFunction)
	{
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &prettyFunction[0], (int)prettyFunction.size(), NULL, 0);
		std::wstring wide(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &prettyFunction[0], (int)prettyFunction.size(), &wide[0], size_needed);
		return methodName(wide);
	}

	std::wstring className(const std::string& prettyFunction)
	{
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &prettyFunction[0], (int)prettyFunction.size(), NULL, 0);
		std::wstring wide(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &prettyFunction[0], (int)prettyFunction.size(), &wide[0], size_needed);
		return className(wide);
	}

	void throwException(const std::wstring& p_errorLine) noexcept(false)
	{
		spk::cout << "Critical exception raised : " << std::endl << p_errorLine << std::endl;
		throw std::runtime_error(spk::wstringToString(p_errorLine).c_str());
	}

	int getLastSocketErrorValue()
	{
		return (WSAGetLastError());
	}
	
	void printCallStack()
	{
		spk::cout.lock();

		const int max_frames = 64;
		void* frames[max_frames];
		HANDLE process = GetCurrentProcess();

		SymInitialize(process, NULL, TRUE);

		int num_frames = CaptureStackBackTrace(0, max_frames, frames, NULL);
		SYMBOL_INFO* symbol = (SYMBOL_INFO*) calloc(sizeof(SYMBOL_INFO) + 256, 1);
		symbol->MaxNameLen = 255;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

		for(int i = 0; i < num_frames; ++i) {
			SymFromAddr(process, (DWORD64)(frames[i]), 0, symbol);
			spk::cout << "[" << num_frames - i - 1 << "] " << symbol->Name << " at 0x" << std::hex << (void*)symbol->Address << std::dec << std::endl;
		}

		free(symbol);
		spk::cout.unlock();
	}
}