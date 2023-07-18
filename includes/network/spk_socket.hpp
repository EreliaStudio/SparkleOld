#pragma once

#ifdef _WIN32
	#include "network/windows/spk_socket.hpp"
#elif __linux__
	#include "network/linux/spk_socket.hpp"
#endif