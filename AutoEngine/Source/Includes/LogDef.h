#pragma once
#include <string>
#include <cstdio>
#include <iostream>
#include <tchar.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "AutoDef.h"

namespace Auto3D {

enum class LogType
{
	/// LogType used for Errors.
	Error = 0,
	/// LogType used for Asserts. (These indicate an error inside Unity itself.)
	Assert = 1,
	/// LogType used for Warnings.
	Warning = 2,
	/// LogType used for regular log messages.
	Log = 3,
	/// LogType used for Exceptions.
	Exception = 4,
	/// LogType used for Debug.
	Debug = 5,
	///
	NumLevels
};

inline const char* LogTypeToString(LogType type)
{
	switch (type)
	{
	case LogType::Assert:    return "Assert";
	case LogType::Debug:     return "Debug";
	case LogType::Exception: return "Exception";
	case LogType::Error:     return "Error";
	case LogType::Log:       return "Log";
	case LogType::Warning:   return "Warning";
	default:                return "";
	}
}

/**
* Define output conslon
*/
#define MAX_STRING 512
#define DebugStringToFileW(_X ,_For)\
do { \
	wchar_t debugChar[MAX_STRING]; \
	wsprintfW(debugChar, L"%s(%d) : %s : %s\n", _T(__FILE__), __LINE__, LogTypeToString(_For),_X); \
	OutputDebugStringW(debugChar); \
}while(0)
#define DebugStringToFileA(_X ,_For)\
do { \
	char debugChar[MAX_STRING]; \
	sprintf(debugChar, "%s(%d) : %s : %s\n", __FILE__, __LINE__,LogTypeToString(_For) ,_X); \
	OutputDebugStringA(debugChar); \
}while(0)


#if AUTO_WIN32_CONSOLE
#	define DebugStringToFile(_For,_T)	do { std::cout << __FILE__ << "(" << __LINE__ << ") : " << LogTypeToString(_T) << " : " << _For <<  std::endl; } while(0)
#else
#	define DebugStringToFile(_X ,_For) DebugStringToFileA(_X ,_For)
#endif

#define ErrorIfString(_Te,_X)	do { if (_Te) DebugStringToFile (_X,LogType::Error); } while(0)
#define ErrorString(_X)			do { DebugStringToFile (_X,LogType::Error); }while(0)
#define WarningIfString(_Te,_X)	do { if (_Te) DebugStringToFile (_X,LogType::Warning);}while(0)
#define WarningString(_X)		do { DebugStringToFile (_X,LogType::Warning);}while(0)	
#define LogString(_X)			do { DebugStringToFile(_X,LogType::Log);} while(0)
#define Print(_X)				do { DebugStringToFile(_X,LogType::Log); }while(0)
#define print(_X)				do { DebugStringToFile(_X,LogType::Log); }while(0)

#define AutoWarningCout	std::cout <<  __FILE__ << "(" << __LINE__ << ") : " << LogTypeToString(LogType::Warning) << " : "
#define AutoErrorCout	std::cout <<  __FILE__ << "(" << __LINE__ << ") : " << LogTypeToString(LogType::Error) << " : "
#define AutoCout		std::cout <<  __FILE__ << "(" << __LINE__ << ") : " << LogTypeToString(LogType::Log) << " : "
#define AutoWCout		std::wcout <<  __FILE__ << L"(" << __LINE__ << L") : " << LogTypeToString(LogType::Log) << L" : "
#define AutoEndl		std::endl


}
