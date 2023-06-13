#pragma once

#ifdef _WIN32
	#include "windows/spk_API_module.hpp"
#elif __linux__
	#include "linux/spk_API_module.hpp"
#endif