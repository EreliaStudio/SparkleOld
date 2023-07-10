#pragma once

#ifdef _WIN32
	#include "windows/spk_window_module.hpp"
#elif __linux__
	#include "linux/spk_window_module.hpp"
#endif