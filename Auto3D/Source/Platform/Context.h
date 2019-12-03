#pragma once

#include "AutoConfig.h"

#ifdef AUTO_OPENGL
#if defined(AUTO_WIN32)
#		include "Windows/WinOGLContext.h"
#elif defined(AUTO_SDL)
#		include "Android/AndroidOGLContext.h"
#	endif
#else
	
#endif