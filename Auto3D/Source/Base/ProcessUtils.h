#pragma once

#include "String.h"

#include <cstdlib>

namespace Auto3D
{

class Mutex;

/// Initialize the FPU to round-to-nearest, single precision mode.
AUTO_API void InitFPU();
/// Display an error dialog with the specified title and message.
AUTO_API void ErrorDialog(const String& title, const String& message);
/// Exit the application with an error message to the console.
AUTO_API void ErrorExit(const String& message = String::EMPTY, int exitCode = EXIT_FAILURE);
/// Open a console window.
AUTO_API void OpenConsoleWindow();
/// Print Unicode text to the console. Will not be printed to the MSVC output window.
AUTO_API void PrintUnicode(const String& str, bool error = false);
/// Print Unicode text to the console with a newline appended. Will not be printed to the MSVC output window.
AUTO_API void PrintUnicodeLine(const String& str, bool error = false);
/// Print ASCII text to the console with a newline appended. Uses printf() to allow printing into the MSVC output window.
AUTO_API void PrintLine(const String& str, bool error = false);
/// Print ASCII text to the console with a newline appended. Uses printf() to allow printing into the MSVC output window.
AUTO_API void PrintLine(const char* str, bool error = false);
/// Parse arguments from the command line. First argument is by default assumed to be the executable name and is skipped.
AUTO_API const Vector<String>& ParseArguments(const String& cmdLine, bool skipFirstArgument = true);
/// Parse arguments from the command line.
AUTO_API const Vector<String>& ParseArguments(const char* cmdLine);
/// Parse arguments from a wide char command line.
AUTO_API const Vector<String>& ParseArguments(const WString& cmdLine);
/// Parse arguments from a wide char command line.
AUTO_API const Vector<String>& ParseArguments(const wchar_t* cmdLine);
/// Parse arguments from argc & argv.
AUTO_API const Vector<String>& ParseArguments(int argc, char** argv);
/// Return previously parsed arguments.
AUTO_API const Vector<String>& GetArguments();
/// Read input from the console window. Return empty if no input.
AUTO_API String GetConsoleInput();
/// Return the runtime platform identifier, or (?) if not identified.
AUTO_API String GetPlatform();
/// Return the number of physical CPU cores.
AUTO_API unsigned GetNumPhysicalCPUs();
/// Return the number of logical CPUs (different from physical if hyperthreading is used.)
AUTO_API unsigned GetNumLogicalCPUs();
/// Set minidump write location as an absolute path. If empty, uses default (UserProfile/AppData/Roaming/urho3D/crashdumps) Minidumps are only supported on MSVC compiler.
AUTO_API void SetMiniDumpDir(const String& pathName);
/// Return minidump write location.
AUTO_API String GetMiniDumpDir();
/// Return the total amount of usable memory in bytes.
AUTO_API unsigned long long GetTotalMemory();
/// Return the name of the currently logged in user, or (?) if not identified.
AUTO_API String GetLoginName();
/// Return the name of the running machine.
AUTO_API String GetHostName();
/// Return the version of the currently running OS, or (?) if not identified.
AUTO_API String GetOSVersion();

}
