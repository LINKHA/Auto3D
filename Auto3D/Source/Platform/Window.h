#pragma once

#include "AutoConfig.h"

#if defined(AUTO_WIN32)
#	include "Windows/WinWindow.h"
#elif defined(AUTO_SDL)
#	include "Android/AndroidWindow.h"
#endif