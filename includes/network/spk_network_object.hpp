#pragma once

#ifdef _WIN32
	#include "network/windows/spk_network_object.hpp"
#elif __linux__
	#include "network/linux/spk_network_object.hpp"
#endif