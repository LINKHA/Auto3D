#pragma once
#include "Auto.h"
#include "ConslonDef.h"
#if defined(_WIN32)
#	include <windows.h>
#endif
#ifdef _MSC_VER
#	include <crtdbg.h>
#endif




#if defined(_MSC_VER) && defined(_DEBUG) && defined(AUTO_WIN32_CONSOLE)
#	define AUTO_MAIN(function) \
		SELECT_DEDICATED_GRAPHICS_CARD \
		int main(int argc, char** argv) \
		{ \
			DETECT_MEMORY_LEAKS();\
			int flag = function;\
			_CrtDumpMemoryLeaks();\
			return flag;\
		}
#elif defined(_MSC_VER) && defined(_DEBUG) && !defined(AUTO_WIN32_CONSOLE)
#	define AUTO_MAIN(_function) \
		SELECT_DEDICATED_GRAPHICS_CARD \
		int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) \
		{ \
			g_hInstance = hInstance; g_prevInstance = prevInstance; g_cmdLine = cmdLine; g_showCmd = showCmd;\
			DETECT_MEMORY_LEAKS();\
			int flag = _function;\
			_CrtDumpMemoryLeaks();\
			return flag;\
		}
#elif defined(_MSC_VER) && AUTO_RELEASE &&!AUTO_WIN32_CONSOLE
#	define AUTO_MAIN(_function) \
		HINSTANCE g_hInstance;HINSTANCE g_prevInstance;PSTR g_cmdLine;int g_showCmd;\
		SELECT_DEDICATED_GRAPHICS_CARD \
		int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) \
		{ \
			g_hInstance = hInstance; g_prevInstance = prevInstance; g_cmdLine = cmdLine; g_showCmd = showCmd;\
			int flag = _function;\
			return flag;\
		}
#elif defined(__ANDROID__) || defined(IOS)
#	define AUTO_MAIN(_function) \
		SELECT_DEDICATED_GRAPHICS_CARD \
		extern "C" __attribute__((visibility("default"))) int SDL_main(int argc, char** argv); \
		int SDL_main(int argc, char** argv) \
		{ \
			int flag = _function;\
			return flag;\
		}
#else
#	define AUTO_MAIN(_function) \
		SELECT_DEDICATED_GRAPHICS_CARD \
		int main(int argc, char** argv) \
		{ \
			int flag = _function;\
			return flag;\
		}
#endif

