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
			int _flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); \
			_flags |= _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF;\
			_CrtSetDbgFlag(_flags);\
		}while(0)
#	endif
#	define DUMP_MEMORY_LEAKS() _CrtDumpMemoryLeaks()
#else
#	define DETECT_MEMORY_LEAKS()
#	define DUMP_MEMORY_LEAKS()
#endif
