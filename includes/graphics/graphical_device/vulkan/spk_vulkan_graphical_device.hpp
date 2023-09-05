#pragma once

#include "spk_system_define.hpp"
#include <vulkan/vulkan.hpp>

#ifdef _WIN32
	#include "graphics/graphical_device/vulkan/windows/spk_vulkan_windows_graphical_device.hpp"
#elif __linux__
	#include "graphics/graphical_device/vulkan/linux/spk_vulkan_linux_graphical_device.hpp"
#endif