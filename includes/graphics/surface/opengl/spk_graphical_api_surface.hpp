#pragma once

#ifdef _WIN32
	#include "windows/spk_graphical_api_surface.hpp"
#elif __linux__
	#include "linux/spk_graphical_api_surface.hpp"

#endif