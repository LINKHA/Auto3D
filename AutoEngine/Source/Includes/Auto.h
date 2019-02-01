#pragma once
#include "AutoDef.h"
#include "AutoSTL.h"
#include "AutoMath.h"
#ifndef AUTO_API
#    ifdef DLL_EXPORTS
/* We are building this library */
#      define AUTO_API __declspec(dllexport)
#    else
/* We are using this library */
#      define AUTO_API __declspec(dllimport)
#    endif
#endif

#if _WIN32
#	define AUTO_EXPORT_DLL __declspec(dllexport)
#elif IOS
#	define AUTO_EXPORT_DLL __attribute__((visibility("default")))
#else
#	define AUTO_EXPORT_DLL
#endif

// Use NVIDIA Dedicated Graphics Card
#define _NVIDIA_USE extern "C"{AUTO_EXPORT_DLL unsigned long NvOptimusEnablement = 0x00000001;}
// Use AMD Dedicated Graphics Card
#define _AMD_USE extern "C"{AUTO_EXPORT_DLL int AmdPowerXpressRequestHighPerformance = 1;}


#if _OPENGL_4_6_
#	define SELECT_DEDICATED_GRAPHICS_CARD _NVIDIA_USE
#else
#	define SELECT_DEDICATED_GRAPHICS_CARD
#endif


