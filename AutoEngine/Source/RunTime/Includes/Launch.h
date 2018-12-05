#pragma once
#include "Auto.h"
#if defined(_WIN32)
#	include <windows.h>
#endif
#ifdef _MSC_VER
#	include <crtdbg.h>
#endif


#if defined(_MSC_VER) && defined(_DEBUG) && AUTO_DEBUG && AUTO_WIN32_CONSOLE
#	define AUTO_MAIN(function) \
		SELECT_DEDICATED_GRAPHICS_CARD \
		int main(int argc, char** argv) \
		{ \
			DETECT_MEMORY_LEAKS();\
			return function;\
		}
#elif defined(_MSC_VER) && defined(_DEBUG)&& AUTO_DEBUG && !AUTO_WIN32_CONSOLE
#	define AUTO_MAIN(function) \
		SELECT_DEDICATED_GRAPHICS_CARD \
		int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) \
		{ \
			DETECT_MEMORY_LEAKS();\
			return function;\
		}
#elif defined(_MSC_VER) && AUTO_RELEASE &&!AUTO_WIN32_CONSOLE
#	define AUTO_MAIN(function) \
		SELECT_DEDICATED_GRAPHICS_CARD \
		int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) \
		{ \
			return function;\
		}
#elif defined(__ANDROID__) || defined(IOS)
#	define AUTO_MAIN(function) \
		SELECT_DEDICATED_GRAPHICS_CARD \
		extern "C" __attribute__((visibility("default"))) int SDL_main(int argc, char** argv); \
		int SDL_main(int argc, char** argv) \
		{ \
			return function; \
		}
#else
#	define AUTO_MAIN(function) \
		SELECT_DEDICATED_GRAPHICS_CARD \
		int main(int argc, char** argv) \
		{ \
			return function;\
		}

#endif 