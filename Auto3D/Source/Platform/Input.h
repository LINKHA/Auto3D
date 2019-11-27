#pragma once

#include "AutoConfig.h"

#ifdef _WIN32 || _WIN64
#	include "Windows/WinInput.h"
#else
#	include "Android/AndroidInput.h"
#endif