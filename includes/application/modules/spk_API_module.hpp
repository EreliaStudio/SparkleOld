#pragma once

#ifdef _WIN32
	#include "windows/spk_api_module.hpp"
#elif __linux__
	#include "linux/spk_api_module.hpp"
#endif