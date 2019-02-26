#pragma once

#include "../AutoConfig.h"
#include "../Base/String.h"

#include <cstdlib>

namespace Auto3D
{

/// Exit the application with an error message to the console.
AUTO_API void ErrorExit(const String& message = String::EMPTY, int exitCode = EXIT_FAILURE);
/// Open a console _window.
AUTO_API void OpenConsoleWindow();
/// Print Unicode text to the console. Will not be printed to the MSVC output _window.
AUTO_API void PrintUnicode(const String& str, bool error = false);
/// Print Unicode text to the console with a newline appended. Will not be printed to the MSVC output _window.
AUTO_API void PrintUnicodeLine(const String& str, bool error = false);
/// Print ASCII text to the console with a newline appended. Uses printf() to allow printing into the MSVC output _window.
AUTO_API void PrintLine(const String& str, bool error = false);
/// Read input from the console _window. Return empty if no input.
AUTO_API String ReadLine();

}
