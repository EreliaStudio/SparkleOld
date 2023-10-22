#pragma once

#ifdef _WIN32
	#include "windows/spk_surface.hpp"
#elif __linux__
	#include "linux/spk_surface.hpp"

#endif