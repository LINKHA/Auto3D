#pragma once
#include "Auto.h"
#if defined(_MSC_VER) && defined(_DEBUG)
#	ifdef _malloca
#		undef _malloca
#	endif
#	if AUTO_DEBUG
#		define _CRTDBG_MAP_ALLOC
#		include <crtdbg.h>
#		include <stdlib.h>
#		define AUTO_DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#		define new AUTO_DEBUG_NEW
#	endif //AUTO_DEBUG
#endif //defined(_MSC_VER) && defined(_DEBUG)

namespace Auto3D {
	void DetectMemoryLeaks(bool enable);
}
