#pragma once

#include <gl/glew.h>
#include <gl/wglew.h>

#ifdef _WIN32
	#include "graphics/graphical_device/opengl/windows/spk_opengl_windows_graphical_device.hpp"
#elif __linux__
	#include "graphics/graphical_device/opengl/linux/spk_opengl_linux_graphical_device.hpp"
#endif