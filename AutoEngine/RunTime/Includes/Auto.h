#ifndef AUTO_H_
#define AUTO_H_

#include <assert.h>

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

//Assert
#define AssertIf(x) assert(!(x))


//Alarm level 
#define AU_ERROR -1
#define AU_NORMAL 0
#define AU_WARNNING 1


//Define Data type
#define Int8 char 
#define PInt8 Int8*
#define UInt8 unsigned Int8
#define PUInt8 UInt8*

#define Int16 short
#define PInt16 Int16*
#define UInt16 unsigned Int16
#define PUInt16 UInt16*

#define Int32 int
#define PInt32 Int32*
#define UInt32 unsigned Int32
#define PUInt32 UInt32*

#define Int64 __int64
#define PInt64 Int64*
#define UInt64 unsigned Int64
#define PUInt64 UInt64*






#endif // BOT_H_
