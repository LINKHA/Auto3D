#pragma once

/// %Auto3D engine namespace.
namespace Auto3D
{
}

#ifdef _MSC_VER
#	pragma warning(disable:4481)
#endif

// Shared library exports
/* #undef AUTO_SHARED */
#if defined(_WIN32) && defined(AUTO_SHARED)
#	ifdef _MSC_VER
#		pragma warning(disable:4251)
#	endif
#	ifdef AUTO_EXPORTS
#		define AUTO_API __declspec(dllexport)
#	else
#		define AUTO_API __declspec(dllimport)
#	endif
#else
#	define AUTO_API
#endif
/* #undef AUTO_NVIDIA */
/* #undef AUTO_AMD */

#if defined(AUTO_NVIDIA)
#	define SELECT_DEDICATED_GRAPHICS_CARD extern "C"{__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;}
#elif defined(AUTO_AMD)
#	define SELECT_DEDICATED_GRAPHICS_CARD extern "C"{__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;}
#else
#	define SELECT_DEDICATED_GRAPHICS_CARD
#endif

// Auto3D build configuration
/* #undef AUTO_LOGGING_L1 */
#define AUTO_LOGGING_L2
#define AUTO_PROFILING
#define AUTO_OPENGL
/* #undef AUTO_DIRECT3D_12 */
#define AUTO_MEMORY_DEBUG
#define AUTO_WIN32_CONSOLE
