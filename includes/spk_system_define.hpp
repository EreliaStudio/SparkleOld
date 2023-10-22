#pragma once

#ifdef _WIN32
	#ifndef UNICODE
	#define UNICODE
	#endif

	#ifndef STRICT
	#define STRICT
	#endif
	
	#ifndef NOMINMAX
	#define NOMINMAX
	#endif
#endif

#if GRAPHICAL_API == 0
	#include <GL/glew.h>
#endif

#ifdef _WIN32

	#if GRAPHICAL_API == 0
		#include <GL/wglew.h>
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
	#include <X11/Xlib.h>
	#include <X11/Xlib-xcb.h>

	// Need to undef the following macros to avoid conflicts with the XLib
	#undef Bool
	typedef int Bool;
	#undef CursorShape
	const int CursorShape = 0;
	#undef Expose
	const int Expose = 12;
	#undef KeyPress
	const int KeyPress = 2;
	#undef KeyRelease
	const int KeyRelease = 3;
	#undef FocusIn
	const int FocusIn = 9;
	#undef FocusOut
	const int FocusOut = 10;
	#undef FontChange
	const int FontChange = 255;
	#undef None
	const long None = 0L;
	#undef Status
	typedef int Status;
	#undef Unsorted
	const int Unsorted = 0;
	#undef Button1
	const int Button1 = 1;
	#undef Button2
	const int Button2 = 2;
	#undef Button3
	const int Button3 = 3;
	#undef Button4
	const int Button4 = 4;
	#undef Button5
	const int Button5 = 5;
	#undef Success
	const int Success = 0;
	#undef Complex
	const int Complex = 0;

	using SystemMessage = xcb_generic_event_t *;
#else
#   error "Unknown compiler"
#endif

#ifdef _WIN32

#elif __linux__

#endif