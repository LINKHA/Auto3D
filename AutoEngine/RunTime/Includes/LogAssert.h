#pragma once
#include <string>
#include <cstdio>
#include <iostream>
#include <Assert.h>

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
#define DebugStringToFile(format,type)	do { std::cout << __FILE__ << "(" << __LINE__ << ") : " << LogTypeToString(type) << " : " << format <<  std::endl; } while(0)
#define ErrorIfString(term,x)	do { if (term) DebugStringToFile (x,LogType::kError); } while(0)
#define ErrorString(x)			do { DebugStringToFile (x,LogType::kError); }while(0)
#define WarningIfString(term,x)	do { if (term) DebugStringToFile (x,LogType::kWarning)}while(0)
#define WarningString(x)		do { DebugStringToFile (x,LogType::kWarning);}while(0)	
#define LogString(x)			do { DebugStringToFile(x,LogType::kLog);} while(0)
#define Print(x)				do { DebugStringToFile(x,LogType::kLog); }while(0)

#define Assert(condition){ \
	if(!(condition)){ \
		ErrorString("The breakpoint is triggered.");\
		exit(EXIT_FAILURE);\
	}\
}

#define AssertString(condition,msg){ \
	if(!(condition)){ \
		ErrorString(msg);\
		exit(EXIT_FAILURE);\
	}\
}

}
