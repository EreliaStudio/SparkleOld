#pragma once

#include "graphics/pipeline/spk_default_shader.hpp"

#if GRAPHICAL_API == 0
	#include "opengl/spk_pipeline.hpp"
#elif GRAPHICAL_API == 1
	#include "vulkan/spk_pipeline.hpp"
#else
    #error "Invalid value for GRAPHICAL_API"
#endif