#pragma once

#pragma once

#ifdef _WIN32
	#include "windows/spk_window.hpp"
#elif __linux__
	#include "linux/spk_window.hpp"
#endif