#pragma once

#ifdef _WIN32
	#include "window/windows/spk_window.hpp"
#elif __linux__
	#include "window/linux/spk_window.hpp"
#endif