#pragma once

#include "AutoConfig.h"
#if defined(AUTO_WIN32)
#	include "Windows/WinInput.h"
#elif defined(AUTO_SDL)
#	include "Android/AndroidInput.h"
#endif