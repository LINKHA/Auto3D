#pragma once
#include "AutoConfig.h"

#ifdef  AUTO_MEMORY_DEBUG
#	ifdef AUTO_WIN32_CONSOLE
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



#ifdef AUTO_WIN32_CONSOLE
#	pragma comment(linker, "/subsystem:console /ENTRY:mainCRTStartup")
#else
#	ifdef _MSC_VER
#		pragma comment(linker, "/subsystem:windows /ENTRY:WinMainCRTStartup")
#	else
#		pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#	endif
#endif