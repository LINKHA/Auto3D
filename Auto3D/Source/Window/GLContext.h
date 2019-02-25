#pragma once
#include "../AutoConfig.h"

#ifdef AUTO_DIRECT3D_12
#	include "Win32/Win32GLContext.h"
#endif

#ifdef AUTO_OPENGLs
#	include "SDL2/SDL2GLContext.h"
#endif


#include "Win32/Win32GLContext.h"