#pragma once

#ifdef _WIN32
	#include "concrete/windows/spk_graphical_application.hpp"
#elif __linux__
	#include "concrete/linux/spk_graphical_application.hpp"
#endif