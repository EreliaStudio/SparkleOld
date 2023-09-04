#pragma once

#include <gl/glew.h>
#include <gl/wglew.h>

#ifdef _WIN32
	#include "graphics/surface/opengl/windows/spk_opengl_windows_surface.hpp"
#elif __linux__
	#include "graphics/surface/opengl/linux/spk_opengl_linux_surface.hpp"
#endif