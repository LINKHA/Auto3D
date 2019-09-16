#pragma once

#include "../AutoConfig.h"

#ifdef AUTO_OPENGL
#	ifdef _WIN32 || _WIN64
#		include "Windows/WinOGLContext.h"
#	else
#		include "Android/AndroidOGLContext.h"
#	endif
#else
	
#endif