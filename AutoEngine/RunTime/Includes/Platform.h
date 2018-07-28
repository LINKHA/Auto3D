#pragma once
#include "Auto.h"
_String GetPlatform()
{
#if defined(__ANDROID__)
	return "Android";
#elif defined(IOS)
	return "iOS";
#elif defined(TVOS)
	return "tvOS";
#elif defined(__APPLE__)
	return "macOS";
#elif defined(_WIN32)
	return "Windows";
#elif defined(RPI)
	return "Raspberry Pi";
#elif defined(__EMSCRIPTEN__)
	return "Web";
#elif defined(__linux__)
	return "Linux";
#else
	return "(?)";
#endif
}