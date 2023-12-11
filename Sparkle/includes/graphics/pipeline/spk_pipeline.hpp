#pragma once

#if GRAPHICAL_API == 0
	#include "opengl/spk_pipeline.hpp"
#elif GRAPHICAL_API == 1
	#include "vulkan/spk_pipeline.hpp"
#else
    #error "Invalid value for GRAPHICAL_API"
#endif