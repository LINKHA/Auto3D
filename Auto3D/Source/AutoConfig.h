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


// Auto3D build configuration
/* #undef AUTO_LOGGING_L1 */
#define AUTO_LOGGING_L2
#define AUTO_PROFILING
#define AUTO_OPENGL
/* #undef AUTO_OPENGL_ES */
/* #undef AUTO_VULKAN */
/* #undef AUTO_DIRECT3D_12 */
/* #undef AUTO_MEMORY_DEBUG */
#define AUTO_WIN32_CONSOLE

#ifndef AUTO_OPENGL
#	ifdef AUTO_OPENGL_ES
#		define AUTO_OPENGL
#	endif
#endif
