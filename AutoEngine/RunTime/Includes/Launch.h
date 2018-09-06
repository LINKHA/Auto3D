#pragma once
#if defined(_WIN32)
#include <Windows.h>
#ifdef _MSC_VER
#include <crtdbg.h>
#endif //_MSC_VER
#endif //_WIN32

#if defined(_MSC_VER) && defined(_DEBUG)
#define AUTO_MAIN(function) \
SELECT_DEDICATED_GRAPHICS_CARD \
int main(int argc, char** argv) \
{ \
	return function;\
}
#endif

#if defined(__ANDROID__) || defined(IOS)
#define AUTO_MAIN(function) \
SELECT_DEDICATED_GRAPHICS_CARD \
extern "C" __attribute__((visibility("default"))) int SDL_main(int argc, char** argv); \
int SDL_main(int argc, char** argv) \
{ \
    return function; \
}
#endif 