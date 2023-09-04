#pragma once

namespace spk
{
	enum class GraphicalAPI
	{
		OpenGL,
		Vulkan
	};
}

#ifdef _WIN32
	#ifndef UNICODE
	#define UNICODE
	#endif

	#ifndef NOMINMAX
	#define NOMINMAX
	#endif
	
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include <iphlpapi.h>
	
	#include <windows.h>

	#pragma comment(lib, "Ws2_32.lib")

	#include "data_structure/spk_pool.hpp"
	#include "data_structure/spk_data_buffer.hpp"

	using SystemMessage = spk::Pool<spk::DataBuffer>::Object;

#elif __linux__
	#include <xcb/xcb.h>

	using SystemMessage = xcb_generic_event_t *;
#else
#   error "Unknown compiler"
#endif

#ifdef _WIN32

#elif __linux__

#endif