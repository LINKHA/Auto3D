#pragma once
#include "AutoDef.h"
#include "AutoSTL.h"

#ifndef AUTO_API
#    ifdef DLL_EXPORTS
/* We are building this library */
#      define AUTO_API __declspec(dllexport)
#    else
/* We are using this library */
#      define AUTO_API __declspec(dllimport)
#    endif
#endif

#if _WIN32
#	define AUTO_EXPORT_DLL __declspec(dllexport)
#elif IOS
#	define AUTO_EXPORT_DLL __attribute__((visibility("default")))
#else
#	define AUTO_EXPORT_DLL
#endif

// Use NVIDIA Dedicated Graphics Card
#define _NVIDIA_USE extern "C"{AUTO_EXPORT_DLL unsigned long NvOptimusEnablement = 0x00000001;}
// Use AMD Dedicated Graphics Card
#define _AMD_USE extern "C"{AUTO_EXPORT_DLL int AmdPowerXpressRequestHighPerformance = 1;}


#if _OPENGL_4_6_
#	define SELECT_DEDICATED_GRAPHICS_CARD _NVIDIA_USE
#endif
#if _OPENGL_4_PLUS_ || _OPENGL_3_PLUS_
#	define SELECT_DEDICATED_GRAPHICS_CARD
#endif


#if  AUTO_MEMORY_DEBUG
#	if AUTO_WIN32_CONSOLE
#		define DETECT_MEMORY_LEAKS() \
		do{\
			int flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); \
			flags |= _CRTDBG_LEAK_CHECK_DF; \
			_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE); \
			_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT); \
			_CrtSetDbgFlag(flags);\
		}while(0)
#	else
#		define DETECT_MEMORY_LEAKS() \
		do{\
			int flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); \
			flags |= _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF;\
			_CrtSetDbgFlag(flags);\
		}while(0)
#	endif
#else
#	define DETECT_MEMORY_LEAKS()
#endif


//
//#if AUTO_WIN32_CONSOLE
//#	pragma comment(linker, "/subsystem:console /ENTRY:mainCRTStartup")
//#else
//#	ifdef _MSC_VER
//#		pragma comment(linker, "/subsystem:windows /ENTRY:WinMainCRTStartup")
//#	else
//#		pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
//#	endif
//#endif
