#pragma once

#ifdef _WIN32
    using SystemMessage = void;

#elif __linux__
	#include <xcb/xcb.h>
	#include <xcb/xcb_keysyms.h>

    using SystemMessage = xcb_generic_event_t;
#else
#   error "Unknown compiler"
#endif

#ifdef _WIN32

#elif __linux__

#endif