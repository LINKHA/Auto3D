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

/*Windows API is not set for security temporarily regardless of platform
#if defined(WIN32) | defined(WIN64)
#	define AUTO_WIN32
#elif  defined(ANDROID) || defined(__ANDROID__)
#	define AUTO_SDL
#endif
*/

#define AUTO_SDL

// Auto3D build configuration
/* #undef AUTO_LOGGING_L1 */
#define AUTO_LOGGING_L2
/* #undef AUTO_PROFILING */
#define AUTO_OPENGL
/* #undef AUTO_OPENGL_ES */
/* #undef AUTO_VULKAN */
/* #undef AUTO_DIRECT3D_12 */
/* #undef AUTO_MEMORY_DEBUG */
/* #undef AUTO_WIN32_CONSOLE */

#ifndef AUTO_OPENGL
#	ifdef AUTO_OPENGL_ES
#		define AUTO_OPENGL
#	endif
#endif
