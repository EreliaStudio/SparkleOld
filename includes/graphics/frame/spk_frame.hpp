#pragma once

#ifdef _WIN32
	#include "windows/spk_frame.hpp"
#elif __linux__
	#include "linux/spk_frame.hpp"

#endif