#pragma once

#define AUTO_DEBUG 1
#define AUTO_MEMORY_DEBUG 1
#define AUTO_RELEASE !AUTO_DEBUG
#define AUTO_WIN32_CONSOLE 1

#define _OPENGL_4_6_ 0		//OpenGL 4.6
#define _OPENGL_4_PLUS_ 1	//OpenGL 4.3
#define _OPENGL_3_PLUS_ 0	//OpenGL 3.3

#define AUTO_OPENGL _OPENGL_4_6_ || _OPENGL_4_PLUS_ || _OPENGL_3_PLUS_

#define DebugCompoundShape 0
#define SharedPtrDebug 1


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