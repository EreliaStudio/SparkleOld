#pragma once

#if GRAPHICAL_API == 0
	#include "opengl/spk_graphical_api_graphical_device.hpp"
#elif GRAPHICAL_API == 1
	#include "vulkan/spk_graphical_api_graphical_device.hpp"
#else
    #error "Invalid value for GRAPHICAL_API"
#endif