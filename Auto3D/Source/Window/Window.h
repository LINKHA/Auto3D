#pragma once
#include "../AutoConfig.h"

#ifdef AUTO_DIRECT3D_12
#	include "Win32/Win32Window.h"
#endif

#ifdef AUTO_OPENGLs
#	include "SDL2/SDL2Window.h"
#endif


#include "Win32/Win32Window.h"