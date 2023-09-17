#pragma once

#if GRAPHICAL_API == 0
    #pragma message("Compiling with OpenGL support.")
    #include "opengl/spk_prototype.hpp"
#elif GRAPHICAL_API == 1
    #pragma message("Compiling with Vulkan support.")
    #include "vulkan/spk_prototype.hpp"
#else
    #error "Invalid value for GRAPHICAL_API"
#endif
