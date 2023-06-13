#pragma once

#ifdef _WIN32
	#ifndef UNICODE
	#define UNICODE
	#endif
	
	#define NOMINMAX
	#include <Windows.h>

	#include "data_structure/spk_pool.hpp"
	#include "data_structure/spk_data_buffer.hpp"

    using SystemMessage = spk::Pool<spk::DataBuffer>::Object;

#elif __linux__
	#include <xcb/xcb.h>
	#include <xcb/xcb_keysyms.h>

    using SystemMessage = xcb_generic_event_t *;
#else
#   error "Unknown compiler"
#endif

#ifdef _WIN32

#elif __linux__

#endif