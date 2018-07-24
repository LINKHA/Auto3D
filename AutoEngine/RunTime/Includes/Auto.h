#pragma once
#include <assert.h>
#include <string>
//namespace AutoEngine
#define AUTO_BEGIN			namespace AutoEngine {
#define AUTO_END			}
#define USING_AUTO			using namespace AutoEngine;
#define AUTO				AutoEngine

//namespace AutoEngineAPI
#define AUTO_API_BEGIN		namespace AutoEngineAPI {
#define AUTO_API_END		}
#define USING_API_AUTO		using namespace AutoEngineAPI;
#define AUTO_API			AutoEngineAPI

//namespace AUMath
#ifndef MATH_BEGIN
	#define MATH_BEGIN		namespace AutoMath {
#endif //!MATH_BEGIN
#ifndef MATH_END
	#define MATH_END		}
#endif//!MATH_END
#ifndef USING_MATH
	#define USING_MATH		using namespace AutoMath;
#endif//!USING_MATH
#ifndef MATH
	#define MATH			AutoMath
#endif//!MATH



//namespace AutoScript
#ifndef SCRIPT_BEGIN
	#define SCRIPT_BEGIN	namespace AutoScript {
#endif //!SCRIPT_BEGIN
#ifndef SCRIPT_END
	#define SCRIPT_END		}
#endif // !SCRIPT_END
#ifndef USING_SCRIPT
	#define USING_SCRIPT	using namespace AutoScript;
#endif // !USING_SCRIPT
#ifndef SCRIPT
	#define SCRIPT			AutoScript
#endif // !SCRIPT



//Define Data type
#define Int8 char 
#define SInt8 signed char 
#define PInt8 Int8*
#define UInt8 unsigned Int8
#define PUInt8 UInt8*

#define Int16 short
#define SInt16 signed short
#define PInt16 Int16*
#define UInt16 unsigned Int16
#define PUInt16 UInt16*

#define Int32 int
#define SInt32 signed int
#define PInt32 Int32*
#define UInt32 unsigned Int32
#define PUInt32 UInt32*

#define Int64 __int64
#define SInt64 signed __int64
#define PInt64 Int64*
#define UInt64 unsigned Int64
#define PUInt64 UInt64*
//
#define _String std::string


#define SAFE_DELETE(p)           do { if(p)	{ delete (p); (p) = nullptr; } } while(0)
#define SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define SAFE_FREE(p)             do { if(p) { free(p); (p) = nullptr; } } while(0)
#define SAFE_RELEASE(p)          do { if(p) { (p)->release(); } } while(0)
#define SAFE_RELEASE_NULL(p)     do { if(p) { (p)->release(); (p) = nullptr; } } while(0)
#define SAFE_RETAIN(p)           do { if(p) { (p)->retain(); } } while(0)
#define BREAK_IF(cond)           if(cond) break

#define ABSTRACT_CLASS class
#define INTERFACE_CLASS class
#define TOOL_CLASS class
//
typedef int objectID;
typedef int classID;

