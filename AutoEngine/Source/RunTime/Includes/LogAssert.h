#pragma once
#include <string>
#include <cstdio>
#include <iostream>
#include <Assert.h>
#include <tchar.h>

namespace Auto3D {

enum class LogType
{
	/// LogType used for Errors.
	kError = 0,
	/// LogType used for Asserts. (These indicate an error inside Unity itself.)
	kAssert = 1,
	/// LogType used for Warnings.
	kWarning = 2,
	/// LogType used for regular log messages.
	kLog = 3,
	/// LogType used for Exceptions.
	kException = 4,
	/// LogType used for Debug.
	kDebug = 5,
	///
	kNumLevels
};

inline const char* LogTypeToString(LogType type)
{
	switch (type)
	{
	case LogType::kAssert:    return "Assert";
	case LogType::kDebug:     return "Debug";
	case LogType::kException: return "Exception";
	case LogType::kError:     return "Error";
	case LogType::kLog:       return "Log";
	case LogType::kWarning:   return "Warning";
	default:                return "";
	}
}
#define DebugStringToFile(_For,_T)	do { std::cout << __FILE__ << "(" << __LINE__ << ") : " << LogTypeToString(_T) << " : " << _For <<  std::endl; } while(0)
#define ErrorIfString(_Te,_X)	do { if (_Te) DebugStringToFile (_X,LogType::kError); } while(0)
#define ErrorString(_X)			do { DebugStringToFile (_X,LogType::kError); }while(0)
#define WarningIfString(_Te,_X)	do { if (_Te) DebugStringToFile (_X,LogType::kWarning);}while(0)
#define WarningString(_X)		do { DebugStringToFile (_X,LogType::kWarning);}while(0)	
#define LogString(_X)			do { DebugStringToFile(_X,LogType::kLog);} while(0)
#define Print(_X)				do { DebugStringToFile(_X,LogType::kLog); }while(0)
#define print(_X)				do { DebugStringToFile(_X,LogType::kLog); }while(0)
#define AutoWarningCout			std::cout <<  __FILE__ << "(" << __LINE__ << ") : " << LogTypeToString(LogType::kWarning) << " : "
#define AutoErrorCout			std::cout <<  __FILE__ << "(" << __LINE__ << ") : " << LogTypeToString(LogType::kError) << " : "
#define AutoCout				std::cout <<  __FILE__ << "(" << __LINE__ << ") : " << LogTypeToString(LogType::kLog) << " : "
#define AutoWCout				std::wcout <<  __FILE__ << "(" << __LINE__ << ") : " << LogTypeToString(LogType::kLog) << " : "
#define AutoEndl				std::endl

#ifdef _WIN32
#	define MAX_STRING 256
#	ifdef _MSC_VER
#		ifdef UNICODE
#			define OutPutString(_X) \
			do { \
				wchar_t debugChar[MAX_STRING]; \
				ZeroMemory(debugChar, MAX_STRING * sizeof(wchar_t)); \
				wsprintfW(debugChar, L"%s(%d) : %s\n", _T(__FILE__), __LINE__, _X); \
				OutputDebugStringW(debugChar); \
			}while(0)
#		else
#			define OutPutString(_X) \
			do { \
				char debugChar[MAX_STRING]; \
				ZeroMemory(debugChar, MAX_STRING * sizeof(wchar_t)); \
				sprintf(debugChar, "%s(%d) : %s\n", __FILE__, __LINE__, _X); \
				OutputDebugStringA(debugChar); \
			}while(0)
#		endif
#	endif
#endif




#define Assert(condition)\
do { \
	if(!(condition)){ \
		ErrorString("The breakpoint is triggered.");\
		assert(condition);\
		exit(EXIT_FAILURE);\
	}\
}while(0)\


#define AssertString(condition,msg)\
do { \
	if(!(condition)){ \
		ErrorString(msg);\
		assert(condition);\
		exit(EXIT_FAILURE);\
	}\
}while(0)\

}
