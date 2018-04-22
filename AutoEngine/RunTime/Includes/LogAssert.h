#ifndef LOG_ASSERT_H_
#define LOG_ASSERT_H_
#include <string>
#include <cstdio>
#include <iostream>
#include <assert.h>
enum LogType
{
	/// LogType used for Errors.
	LogType_Error = 0,
	/// LogType used for Asserts. (These indicate an error inside Unity itself.)
	LogType_Assert = 1,
	/// LogType used for Warnings.
	LogType_Warning = 2,
	/// LogType used for regular log messages.
	LogType_Log = 3,
	/// LogType used for Exceptions.
	LogType_Exception = 4,
	/// LogType used for Debug.
	LogType_Debug = 5,
	///
	LogType_NumLevels
};

inline const char* LogTypeToString(LogType type)
{
	switch (type)
	{
	case LogType_Assert:    return "Assert";
	case LogType_Debug:     return "Debug";
	case LogType_Exception: return "Exception";
	case LogType_Error:     return "Error";
	case LogType_Log:       return "Log";
	case LogType_Warning:   return "Warning";
	default:                return "";
	}
}
#define DebugStringToFile(format,type)	printf("%s(%d) : %s : " format "\n",__FILE__, __LINE__,LogTypeToString(type)) 
#define ErrorIfString(term,x)	do { if (term) DebugStringToFile (x,LogType_Error); } while(0)
#define ErrorString(x)			do { DebugStringToFile (x,LogType_Error); }while(0)
#define WarningIfString(term,x)	do { if (term) DebugStringToFile (x,LogType_Warning)}while(0)
#define WarningString(x)		do { DebugStringToFile (x,LogType_Warning);}while(0)	
#define LogString(x)			do { DebugStringToFile( x,LogType_Log);} while(0)
#define Print(x)				do { std::cout<<x<<std::endl; }while(0)


#define DebugAssertIf(x)	do { (void)sizeof(x); } while(0)
#define DebugAssert(x) 		do { (void)sizeof(x); } while(0)
#define DebugAssertMsg(x, ...) {  }

//Assert
#define AssertIf(x) assert(!(x))
#define Assert(x)	assert(x)

#endif // 