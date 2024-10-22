#pragma once

#if GRAPHICAL_API == 0
	#include "opengl/spk_pipeline_basic_functions.hpp"
#elif GRAPHICAL_API == 1
	#include "vulkan/spk_pipeline_basic_functions.hpp"
#else
    #error "Invalid value for GRAPHICAL_API"
#endif