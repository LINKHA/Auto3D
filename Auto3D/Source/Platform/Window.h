#pragma once

#include "../AutoConfig.h"

#ifdef _WIN32 || _WIN64
#	include "Windows/WinWindow.h"
#else
#	include "Android/AndroidWindow.h"
#endif