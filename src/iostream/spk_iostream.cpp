#include "iostream/spk_iostream.hpp"

namespace spk
{
	thread_local IOStream cout = IOStream(std::wcout);
	thread_local IOStream cerr = IOStream(std::wcerr);
}