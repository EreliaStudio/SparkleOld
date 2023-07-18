#pragma once

#ifdef _WIN32
	#include "network/windows/spk_acceptor.hpp"
#elif __linux__
	#include "network/linux/spk_acceptor.hpp"
#endif