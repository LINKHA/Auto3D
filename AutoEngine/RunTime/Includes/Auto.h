#ifndef AUTO_H_
#define AUTO_H_

#include <assert.h>

//namespace AUTO_Engine
#define AUTO_BEGIN			namespace AutoEngine {
#define AUTO_END			}
#define USING_AUTO			using namespace AutoEngine;
#define AUTO				::AutoEngine

//namespace AUTO_Engine_API
#define AUTO_API_BEGIN		namespace AutoEngineAPI {
#define AUTO_API_END		}
#define USING_API_AUTO		using namespace AutoEngineAPI;
#define AUTO_API			::AutoEngineAPI

//namespace AUMATH
#ifndef MATH_BEGIN
	#define MATH_BEGIN		namespace AUMath {
#endif //!MATH_BEGIN
#ifndef MATH_END
	#define MATH_END		}
#endif//!MATH_END
#ifndef USING_MATH
	#define USING_MATH		using namespace AUMath;
#endif//!USING_MATH
#ifndef MATH
	#define MATH			::AUMath
#endif//!MATH

//Assert
#define AssertIf(x) assert(!(x))


//Alarm level 
#define AU_ERROR -1
#define AU_NORMAL 0
#define AU_WARNNING 1





#endif // BOT_H_
