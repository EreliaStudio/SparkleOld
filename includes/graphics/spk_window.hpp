#pragma once

#pragma once

#include "design_pattern/spk_singleton.hpp"
#include "math/spk_vector2.hpp"
#include "spk_system_define.hpp"

#ifdef _WIN32
	#include "windows/spk_window.hpp"
#elif __linux__
	#include "linux/spk_window.hpp"
#endif