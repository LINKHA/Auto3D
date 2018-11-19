#pragma once
#include "LogAssert.h"
#include "AutoSTL.h"


//namespace AutoScript
#ifndef SCRIPT_BEGIN
#	define SCRIPT_BEGIN	namespace AutoScript {
#endif //!SCRIPT_BEGIN

#ifndef SCRIPT_END
#	define SCRIPT_END		}
#endif // !SCRIPT_END

#ifndef USING_SCRIPT
#	define USING_SCRIPT	using namespace AutoScript;
#endif // !USING_SCRIPT

#ifndef SCRIPT
#	define SCRIPT			AutoScript
#endif // !SCRIPT


#define _OPENGL_4_6_ 0
#define _OPENGL_4_PLUS_ 1	//OpenGL 4.3
#define _OPENGL_3_PLUS_ 0	//OpenGL 3.3

#ifndef AUTO_API
#    ifdef DLL_EXPORTS
/* We are building this library */
#      define AUTO_API __declspec(dllexport)
#    else
/* We are using this library */
#      define AUTO_API __declspec(dllimport)
#    endif
#  endif

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

//Define Data type
using Int8 = char;
using SInt8 = signed char;
using PInt8 = char*;
using UInt8 = unsigned char;
using PUInt8 = unsigned char*;

using Int16 = short;
using SInt16 = signed short;
using PInt16 = short*;
using UInt16 = unsigned short;
using PUInt16 = unsigned short*;

using Int32 = int;
using SInt32 = signed int;
using PInt32 = int*;
using UInt32 = unsigned int;
using PUInt32 = unsigned int*;

using Int64 = __int64;
using SInt64 = signed __int64;
using PInt64 = __int64*;
using UInt64 = unsigned __int64;
using PUInt64 = unsigned __int64*;



#define SAFE_DELETE(_X)           do { if(_X)	{ delete (_X); (_X) = nullptr; } } while(0)
#define SAFE_DELETE_ARRAY(_X)     do { if(_X) { delete[] (_X); (_X) = nullptr; } } while(0)
#define SAFE_FREE(_X)             do { if(_X) { free(_X); (_X) = nullptr; } } while(0)
#define SAFE_RELEASE(_X)          do { if(_X) { (_X)->release(); } } while(0)
#define SAFE_RELEASE_NULL(_X)     do { if(_X) { (_X)->release(); (_X) = nullptr; } } while(0)
#define SAFE_RETAIN(_X)           do { if(_X) { (_X)->retain(); } } while(0)
#define BREAK_IF(_X)			  do { if(_X) break; } while(0)


#define AUTO_DEBUG 1
#define MEMORY_DEBUG 0
#define AUTO_RELEASE !AUTO_DEBUG

#if AUTO_DEBUG 
#else
#	pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif

#if  MEMORY_DEBUG
#	include "DebugNew.h"
#	define DETECT_MEMORY_LEAKS(Enable)	DetectMemoryLeaks(Enable)
#else
#	define DETECT_MEMORY_LEAKS
#endif



#define SharedPtrDebug 0
