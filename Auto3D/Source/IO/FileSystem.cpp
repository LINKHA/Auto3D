#include "../Base/AutoPtr.h"
#include "../Base/String.h"
#include "../Base/Vector.h"
#include "../Base/WString.h"
#include "File.h"
#include "FileSystem.h"
#ifdef __ANDROID__
#	include <SDL_rwops.h>
#endif

#include <SDL_filesystem.h>
#include <cstdio>
#include <cstring>
#include <sys/stat.h>

#ifdef _WIN32
#	ifndef _MSC_VER
#		define _WIN32_IE 0x501
#	endif
#	include <windows.h>
#	include <shellapi.h>
#	include <direct.h>
#	include <shlobj.h>
#	include <sys/types.h>
#	include <sys/utime.h>
#else
#	include <dirent.h>
#	include <cerrno>
#	include <unistd.h>
#	include <utime.h>
#	include <sys/wait.h>
#	define MAX_PATH 256
#endif

#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

#include "../Debug/Log.h"
#include "../Debug/DebugNew.h"

namespace Auto3D
{

int DoSystemCommand(const String& commandLine, bool redirectToLog)
{
#if defined(TVOS) || defined(IOS)
	return -1;
#else
#if !defined(__EMSCRIPTEN__) && !defined(MINI_URHO)
	if (!redirectToLog)
#endif
		return system(commandLine.CString());

#if !defined(__EMSCRIPTEN__) && !defined(MINI_URHO)
	// Get a platform-agnostic temporary file name for stderr redirection
	String stderrFilename;
	String adjustedCommandLine(commandLine);
	char* prefPath = SDL_GetPrefPath("Auto3D", "temp");
	if (prefPath)
	{
		stderrFilename = String(prefPath) + "command-stderr";
		adjustedCommandLine += " 2>" + stderrFilename;
		SDL_free(prefPath);
	}

#ifdef _MSC_VER
#define popen _popen
#define pclose _pclose
#endif

	// Use popen/pclose to capture the stdout and stderr of the command
	FILE* file = popen(adjustedCommandLine.CString(), "r");
	if (!file)
		return -1;

	// Capture the standard output stream
	char buffer[128];
	while (!feof(file))
	{
		if (fgets(buffer, sizeof(buffer), file))
			LogString(buffer);
	}
	int exitCode = pclose(file);

	// Capture the standard error stream
	if (!stderrFilename.IsEmpty())
	{
		SharedPtr<File> errFile(new File(stderrFilename, FileMode::READ));
		while (!errFile->IsEof())
		{
			unsigned numRead = errFile->Read(buffer, sizeof(buffer));
			if (numRead)
				LogString(String(buffer, numRead));
		}
	}

	return exitCode;
#endif
#endif
}


int DoSystemRun(const String& fileName, const Vector<String>& arguments)
{
#ifdef TVOS
	return -1;
#else
	String fixedFileName = NativePath(fileName);

#ifdef _WIN32
	// Add .exe extension if no extension defined
	if (Extension(fixedFileName).IsEmpty())
		fixedFileName += ".exe";

	String commandLine = "\"" + fixedFileName + "\"";
	for (unsigned i = 0; i < arguments.Size(); ++i)
		commandLine += " " + arguments[i];

	STARTUPINFOW startupInfo;
	PROCESS_INFORMATION processInfo;
	memset(&startupInfo, 0, sizeof startupInfo);
	memset(&processInfo, 0, sizeof processInfo);

	WString commandLineW(commandLine);
	if (!CreateProcessW(nullptr, (wchar_t*)commandLineW.CString(), nullptr, nullptr, 0, CREATE_NO_WINDOW, nullptr, nullptr, &startupInfo, &processInfo))
		return -1;

	WaitForSingleObject(processInfo.hProcess, INFINITE);
	DWORD exitCode;
	GetExitCodeProcess(processInfo.hProcess, &exitCode);

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);

	return exitCode;
#else
	pid_t pid = fork();
	if (!pid)
	{
		VECTOR<const char*> argPtrs;
		argPtrs.Push(fixedFileName.CString());
		for (unsigned i = 0; i < arguments.Size(); ++i)
			argPtrs.Push(arguments[i].CString());
		argPtrs.Push(nullptr);

		execvp(argPtrs[0], (char**)&argPtrs[0]);
		return -1; // Return -1 if we could not spawn the process
	}
	else if (pid > 0)
	{
		int exitCode;
		wait(&exitCode);
		return exitCode;
	}
	else
		return -1;
#endif
#endif
}

FileSystem::FileSystem()
{
	RegisterSubsystem(this);
}

FileSystem::~FileSystem()
{
	RemoveSubsystem(this);
}

void FileSystem::RegisterPath(const String& pathName)
{
	if (pathName.IsEmpty())
		return;

	_allowedPaths.Insert(AddTrailingSlash(pathName));
}

bool FileSystem::SetCurrentDir(const String& pathName)
{
#ifdef _WIN32
	if (SetCurrentDirectoryW(WideNativePath(pathName).CString()) == FALSE)
		return false;
#else
	if (chdir(NativePath(pathName).CString()) != 0)
		return false;
#endif

	return true;
}

bool FileSystem::CreateDir(const String& pathName)
{
#ifdef _WIN32
	bool success = (CreateDirectoryW(WideNativePath(RemoveTrailingSlash(pathName)).CString(), 0) == TRUE) ||
		(GetLastError() == ERROR_ALREADY_EXISTS);
#else
	bool success = mkdir(NativePath(RemoveTrailingSlash(pathName)).CString(), S_IRWXU) == 0 || errno == EEXIST;
#endif

	return success;
}

int FileSystem::SystemCommand(const String& commandLine, bool redirectStdOutToLog)
{
	if (_allowedPaths.IsEmpty())
		return DoSystemCommand(commandLine, redirectStdOutToLog);
	else
	{
		ErrorString("Executing an external command is not allowed");
		return -1;
	}
}

int FileSystem::SystemRun(const String& fileName, const Vector<String>& arguments)
{
	if (_allowedPaths.IsEmpty())
		return DoSystemRun(fileName, arguments);
	else
	{
		ErrorString("Executing an external command is not allowed");
		return -1;
	}
}

bool FileSystem::Copy(const String& srcFileName, const String& destFileName)
{
	File srcFile(srcFileName, FileMode::READ);
	if (!srcFile.IsOpen())
		return false;
	File destFile(destFileName, FileMode::WRITE);
	if (!destFile.IsOpen())
		return false;

	/// \todo Should use a fixed-_size buffer to allow copying very large files
	size_t fileSize = srcFile.Size();
	AutoArrayPtr<unsigned char> buffer(new unsigned char[fileSize]);

	size_t bytesRead = srcFile.Read(buffer.Get(), fileSize);
	size_t bytesWritten = destFile.Write(buffer.Get(), fileSize);
	return bytesRead == fileSize && bytesWritten == fileSize;
}

bool FileSystem::Rename(const String& srcFileName, const String& destFileName)
{
#ifdef _WIN32
	return MoveFileW(WideNativePath(srcFileName).CString(), WideNativePath(destFileName).CString()) != 0;
#else
	return rename(NativePath(srcFileName).CString(), NativePath(destFileName).CString()) == 0;
#endif
}

bool FileSystem::Delete(const String& fileName)
{
#ifdef _WIN32
	return DeleteFileW(WideNativePath(fileName).CString()) != 0;
#else
	return remove(NativePath(fileName).CString()) == 0;
#endif
}

String FileSystem::GetCurrentDir()
{
#ifdef _WIN32
	wchar_t path[MAX_PATH];
	path[0] = 0;
	GetCurrentDirectoryW(MAX_PATH, path);
	return AddTrailingSlash(String(path));
#else
	char path[MAX_PATH];
	path[0] = 0;
	getcwd(path, MAX_PATH);
	return AddTrailingSlash(String(path));
#endif
}

unsigned LastModifiedTime(const String& fileName)
{
	if (fileName.IsEmpty())
		return 0;

#ifdef _WIN32
	struct _stat st;
	if (!_stat(fileName.CString(), &st))
		return (unsigned)st.st_mtime;
	else
		return 0;
#else
	struct stat st;
	if (!stat(fileName.CString(), &st))
		return (unsigned)st.st_mtime;
	else
		return 0;
#endif
}

bool SetLastModifiedTime(const String& fileName, unsigned newTime)
{
	if (fileName.IsEmpty())
		return false;

#ifdef WIN32
	struct _stat oldTime;
	struct _utimbuf newTimes;
	if (_stat(fileName.CString(), &oldTime) != 0)
		return false;
	newTimes.actime = oldTime.st_atime;
	newTimes.modtime = newTime;
	return _utime(fileName.CString(), &newTimes) == 0;
#else
	struct stat oldTime;
	struct utimbuf newTimes;
	if (stat(fileName.CString(), &oldTime) != 0)
		return false;
	newTimes.actime = oldTime.st_atime;
	newTimes.modtime = newTime;
	return utime(fileName.CString(), &newTimes) == 0;
#endif
}

bool FileExists(const String& fileName)
{
	String fixedName = NativePath(RemoveTrailingSlash(fileName));

#ifdef _WIN32
	DWORD attributes = GetFileAttributesW(WString(fixedName).CString());
	if (attributes == INVALID_FILE_ATTRIBUTES || attributes & FILE_ATTRIBUTE_DIRECTORY)
		return false;
#else
	struct stat st;
	if (stat(fixedName.CString(), &st) || st.st_mode & S_IFDIR)
		return false;
#endif

	return true;
}

bool DirExists(const String& pathName)
{
#ifndef WIN32
	// Always return true for the root directory
	if (pathName == "/")
		return true;
#endif

	String fixedName = NativePath(RemoveTrailingSlash(pathName));

#ifdef _WIN32
	DWORD attributes = GetFileAttributesW(WString(fixedName).CString());
	if (attributes == INVALID_FILE_ATTRIBUTES || !(attributes & FILE_ATTRIBUTE_DIRECTORY))
		return false;
#else
	struct stat st;
	if (stat(fixedName.CString(), &st) || !(st.st_mode & S_IFDIR))
		return false;
#endif

	return true;
}

static void ScanDirInternal(Vector<String>& result, String path, const String& startPath,
	const String& filter, unsigned flags, bool recursive)
{
	path = AddTrailingSlash(path);
	String deltaPath;
	if (path.Length() > startPath.Length())
		deltaPath = path.Substring(startPath.Length());

	String filterExtension = filter.Substring(filter.Find('.'));
	if (filterExtension.Contains('*'))
		filterExtension.Clear();

#ifdef _WIN32
	WIN32_FIND_DATAW info;
	HANDLE _handle = FindFirstFileW(WString(path + "*").CString(), &info);
	if (_handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			String fileName(info.cFileName);
			if (!fileName.IsEmpty())
			{
				if (info.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN && !(flags & SCAN_HIDDEN))
					continue;
				if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (flags & SCAN_DIRS)
						result.Push(deltaPath + fileName);
					if (recursive && fileName != "." && fileName != "..")
						ScanDirInternal(result, path + fileName, startPath, filter, flags, recursive);
				}
				else if (flags & SCAN_FILES)
				{
					if (filterExtension.IsEmpty() || fileName.EndsWith(filterExtension))
						result.Push(deltaPath + fileName);
				}
			}
		} while (FindNextFileW(_handle, &info));

		FindClose(_handle);
	}
#else
	DIR *dir;
	struct dirent *de;
	struct stat st;
	dir = opendir(NativePath(path).CString());
	if (dir)
	{
		while ((de = readdir(dir)))
		{
			/// \todo Filename may be unnormalized Unicode on Mac OS X. Re-normalize as necessary
			String fileName(de->d_name);
			bool normalEntry = fileName != "." && fileName != "..";
			if (normalEntry && !(_flags & SCAN_HIDDEN) && fileName.StartsWith("."))
				continue;
			String pathAndName = path + fileName;
			if (!stat(pathAndName.CString(), &st))
			{
				if (st.st_mode & S_IFDIR)
				{
					if (_flags & SCAN_DIRS)
						result.Push(deltaPath + fileName);
					if (recursive && normalEntry)
						ScanDirInternal(result, path + fileName, startPath, _filter, _flags, recursive);
				}
				else if (_flags & SCAN_FILES)
				{
					if (filterExtension.IsEmpty() || fileName.EndsWith(filterExtension))
						result.Push(deltaPath + fileName);
				}
			}
		}
		closedir(dir);
	}
#endif
}

void ScanDir(Vector<String>& result, const String& pathName, const String& filter, unsigned flags, bool recursive)
{
	String initialPath = AddTrailingSlash(pathName);
	ScanDirInternal(result, initialPath, initialPath, filter, flags, recursive);
}

String ExecutableDir()
{
	String ret;

#if defined(_WIN32)
	wchar_t exeName[MAX_PATH];
	exeName[0] = 0;
	GetModuleFileNameW(0, exeName, MAX_PATH);
	ret = Path(String(exeName));
#elif defined(__APPLE__)
	char exeName[MAX_PATH];
	memset(exeName, 0, MAX_PATH);
	unsigned _size = MAX_PATH;
	_NSGetExecutablePath(exeName, &_size);
	ret = Path(String(exeName));
#elif defined(__linux__)
	char exeName[MAX_PATH];
	memset(exeName, 0, MAX_PATH);
	pid_t pid = getpid();
	String link = "/proc/" + String(pid) + "/exe";
	readlink(link.CString(), exeName, MAX_PATH);
	ret = Path(String(exeName));
#endif

	// Sanitate /./ construct away
	ret.Replace("/./", "/");
	return ret;
}

void SplitPath(const String& fullPath, String& pathName, String& fileName, String& extension, bool lowercaseExtension)
{
	String fullPathCopy = NormalizePath(fullPath);

	size_t extPos = fullPathCopy.FindLast('.');
	size_t pathPos = fullPathCopy.FindLast('/');

	if (extPos != String::NPOS && (pathPos == String::NPOS || extPos > pathPos))
	{
		extension = fullPathCopy.Substring(extPos);
		if (lowercaseExtension)
			extension = extension.ToLower();
		fullPathCopy = fullPathCopy.Substring(0, extPos);
	}
	else
		extension.Clear();

	pathPos = fullPathCopy.FindLast('/');
	if (pathPos != String::NPOS)
	{
		fileName = fullPathCopy.Substring(pathPos + 1);
		pathName = fullPathCopy.Substring(0, pathPos + 1);
	}
	else
	{
		fileName = fullPathCopy;
		pathName.Clear();
	}
}

String Path(const String& fullPath)
{
	String path, file, extension;
	SplitPath(fullPath, path, file, extension);
	return path;
}

String FileName(const String& fullPath)
{
	String path, file, extension;
	SplitPath(fullPath, path, file, extension);
	return file;
}

String Extension(const String& fullPath, bool lowercaseExtension)
{
	String path, file, extension;
	SplitPath(fullPath, path, file, extension, lowercaseExtension);
	return extension;
}

String FileNameAndExtension(const String& fileName, bool lowercaseExtension)
{
	String path, file, extension;
	SplitPath(fileName, path, file, extension, lowercaseExtension);
	return file + extension;
}

String ReplaceExtension(const String& fullPath, const String& newExtension)
{
	String path, file, extension;
	SplitPath(fullPath, path, file, extension);
	return path + file + newExtension;
}

String AddTrailingSlash(const String& pathName)
{
	String ret = pathName.Trimmed();
	ret.Replace('\\', '/');
	if (!ret.IsEmpty() && ret.Back() != '/')
		ret += '/';
	return ret;
}

String RemoveTrailingSlash(const String& pathName)
{
	String ret = pathName.Trimmed();
	ret.Replace('\\', '/');
	if (!ret.IsEmpty() && ret.Back() == '/')
		ret.Resize(ret.Length() - 1);
	return ret;
}

String ParentPath(const String& path)
{
	size_t pos = RemoveTrailingSlash(path).FindLast('/');
	if (pos != String::NPOS)
		return path.Substring(0, pos + 1);
	else
		return String();
}

String NormalizePath(const String& pathName)
{
	return pathName.Replaced('\\', '/');
}

String NativePath(const String& pathName)
{
#ifdef _WIN32
	return pathName.Replaced('/', '\\');
#else
	return pathName;
#endif
}

WString WideNativePath(const String& pathName)
{
#ifdef _WIN32
	return WString(pathName.Replaced('/', '\\'));
#else
	return WString(pathName);
#endif
}

bool IsAbsolutePath(const String& pathName)
{
	if (pathName.IsEmpty())
		return false;

	String path = NormalizePath(pathName);

	if (path[0] == '/')
		return true;

#ifdef _WIN32
	if (path.Length() > 1 && IsAlpha(path[0]) && path[1] == ':')
		return true;
#endif

	return false;
}

}