#ifdef __ANDROID__
#	include <SDL2/include/SDL_rwops.h>
#endif

#ifndef MINI_AUTO
#	include <SDL/include/SDL_filesystem.h>
#endif

#include <sys/stat.h>
#include <cstdio>

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
#	include <mach-o/dyld.h>
#endif

#include "File.h"
#include "FileSystem.h"

extern "C"
{
#ifdef __ANDROID__
	const char* SDL_Android_GetFilesDir();
	char** SDL_Android_GetFileList(const char* path, int* count);
	void SDL_Android_FreeFileList(char*** array, int* count);
#elif defined(IOS) || defined(TVOS)
	const char* SDL_IOS_GetResourceDir();
	const char* SDL_IOS_GetDocumentsDir();
#endif
}

namespace Auto3D {

int DoSystemCommand(const STRING& commandLine, bool redirectToLog, Ambient* ambient)
{
#if defined(TVOS) || defined(IOS)
	return -1;
#else
#if !defined(__EMSCRIPTEN__) && !defined(MINI_URHO)
	if (!redirectToLog)
#endif
		return system(commandLine.CStr());

#if !defined(__EMSCRIPTEN__) && !defined(MINI_URHO)
	// Get a platform-agnostic temporary file name for stderr redirection
	STRING stderrFilename;
	STRING adjustedCommandLine(commandLine);
	char* prefPath = SDL_GetPrefPath("Auto3D", "temp");
	if (prefPath)
	{
		stderrFilename = STRING(prefPath) + "command-stderr";
		adjustedCommandLine += " 2>" + stderrFilename;
		SDL_free(prefPath);
	}

#ifdef _MSC_VER
#define popen _popen
#define pclose _pclose
#endif

	// Use popen/pclose to capture the stdout and stderr of the command
	FILE* file = popen(adjustedCommandLine.CStr(), "r");
	if (!file)
		return -1;

	// Capture the standard output stream
	char buffer[128];
	while (!feof(file))
	{
		if (fgets(buffer, sizeof(buffer), file))
			LogString(STRING(buffer));
	}
	int exitCode = pclose(file);

	// Capture the standard error stream
	if (!stderrFilename.Empty())
	{
		SharedPtr<File> errFile(new File(ambient, stderrFilename, FileMode::Read));
		while (!errFile->IsEof())
		{
			unsigned numRead = errFile->Read(buffer, sizeof(buffer));
			if (numRead)
				LogString(STRING(buffer, numRead));
		}
	}

	return exitCode;
#endif
#endif
}


int DoSystemRun(const STRING& fileName, const VECTOR<STRING>& arguments)
{
#ifdef TVOS
	return -1;
#else
	STRING fixedFileName = GetNativePath(fileName);

#ifdef _WIN32
	// Add .exe extension if no extension defined
	if (GetExtension(fixedFileName).Empty())
		fixedFileName += ".exe";

	STRING commandLine = "\"" + fixedFileName + "\"";
	for (unsigned i = 0; i < arguments.size(); ++i)
		commandLine += " " + arguments[i];

	STARTUPINFOW startupInfo;
	PROCESS_INFORMATION processInfo;
	memset(&startupInfo, 0, sizeof startupInfo);
	memset(&processInfo, 0, sizeof processInfo);

	WSTRING commandLineW(commandLine);
	if (!CreateProcessW(nullptr, (wchar_t*)commandLineW.CStr(), nullptr, nullptr, 0, CREATE_NO_WINDOW, nullptr, nullptr, &startupInfo, &processInfo))
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

FileSystem::FileSystem(Ambient* ambient)
	:Super(ambient)
{

}

FileSystem::~FileSystem()
{
}

void FileSystem::RegisterPath(const STRING& pathName)
{
	if (pathName.Empty())
		return;

	_allowedPaths.insert(AddTrailingSlash(pathName));
}

bool FileSystem::SetCurrentDir(const STRING& pathName)
{
	if (!CheckAccess(pathName))
	{
		WarningString("Access denied to " + pathName);
		return false;
	}
#ifdef _WIN32
	if (SetCurrentDirectoryW(GetWideNativePath(pathName).CStr()) == FALSE)
	{
		WarningString("Failed to change directory to " + pathName);
		return false;
	}
#else
	if (chdir(GetNativePath(pathName).CStr()) != 0)
	{
		WarningString("Failed to change directory to " + pathName);
		return false;
	}
#endif

	return true;
}


bool FileSystem::CreateDir(const STRING& pathName)
{
	if (!CheckAccess(pathName))
	{
		WarningString("Access denied to " + pathName);
		return false;
	}

	// Create each of the parents if necessary
	STRING parentPath = GetParentPath(pathName);
	if (parentPath.Length() > 1 && !DirExists(parentPath))
	{
		if (!CreateDir(parentPath))
			return false;
	}

#ifdef _WIN32
	bool success = (CreateDirectoryW(GetWideNativePath(RemoveTrailingSlash(pathName)).CStr(), nullptr) == TRUE) ||
		(GetLastError() == ERROR_ALREADY_EXISTS);
#else
	bool success = mkdir(GetNativePath(RemoveTrailingSlash(pathName)).CString(), S_IRWXU) == 0 || errno == EEXIST;
#endif

	if (success)
		LogString("Created directory " + pathName);
	else
		WarningString("Failed to create directory " + pathName);

	return success;
}

int FileSystem::SystemCommand(const STRING& commandLine, bool redirectStdOutToLog)
{
	if (_allowedPaths.empty())
		return DoSystemCommand(commandLine, redirectStdOutToLog, _ambient);
	else
	{
		ErrorString("Executing an external command is not allowed");
		return -1;
	}
}

int FileSystem::SystemRun(const STRING& fileName, const VECTOR<STRING>& arguments)
{
	if (_allowedPaths.empty())
		return DoSystemRun(fileName, arguments);
	else
	{
		ErrorString("Executing an external command is not allowed");
		return -1;
	}
}

bool FileSystem::Copy(const STRING& srcFileName, const STRING& destFileName)
{
	if (!CheckAccess(GetPath(srcFileName)))
	{
		LogString("Access denied to " + srcFileName);
		return false;
	}
	if (!CheckAccess(GetPath(destFileName)))
	{
		LogString("Access denied to " + destFileName);
		return false;
	}

	SharedPtr<File> srcFile(new File(_ambient, srcFileName, FileMode::Read));
	if (!srcFile->IsOpen())
		return false;
	SharedPtr<File> destFile(new File(_ambient, destFileName, FileMode::Write));
	if (!destFile->IsOpen())
		return false;

	unsigned fileSize = srcFile->GetSize();
	SharedArrayPtr<unsigned char> buffer(new unsigned char[fileSize]);

	unsigned bytesRead = srcFile->Read(buffer.get(), fileSize);
	unsigned bytesWritten = destFile->Write(buffer.get(), fileSize);
	return bytesRead == fileSize && bytesWritten == fileSize;
}

bool FileSystem::Rename(const STRING& srcFileName, const STRING& destFileName)
{
	if (!CheckAccess(GetPath(srcFileName)))
	{
		LogString("Access denied to " + srcFileName);
		return false;
	}
	if (!CheckAccess(GetPath(destFileName)))
	{
		LogString("Access denied to " + destFileName);
		return false;
	}

#ifdef _WIN32
	return MoveFileW(GetWideNativePath(srcFileName).CStr(), GetWideNativePath(destFileName).CStr()) != 0;
#else
	return rename(GetNativePath(srcFileName).CStr(), GetNativePath(destFileName).CStr()) == 0;
#endif
}

bool FileSystem::Delete(const STRING& fileName)
{
	if (!CheckAccess(GetPath(fileName)))
	{
		ErrorString("Access denied to " + fileName);
		return false;
	}
#ifdef _WIN32
	return DeleteFileW(GetWideNativePath(fileName).CStr()) != 0;
#else
	return remove(GetNativePath(fileName).CStr()) == 0;
#endif
}

STRING FileSystem::GetUserDocumentsDir()
{
#if defined(__ANDROID__)
	return AddTrailingSlash(SDL_Android_GetFilesDir());
#elif defined(IOS) || defined(TVOS)
	return AddTrailingSlash(SDL_IOS_GetDocumentsDir());
#elif defined(_WIN32)
	wchar_t pathName[MAX_PATH];
	pathName[0] = 0;
	SHGetSpecialFolderPathW(nullptr, pathName, CSIDL_PERSONAL, 0);
	return AddTrailingSlash(STRING(pathName));
#else
	char pathName[MAX_PATH];
	pathName[0] = 0;
	strcpy(pathName, getenv("HOME"));
	return AddTrailingSlash(STRING(pathName));
#endif
}

STRING FileSystem::GetProgramDir()
{
#if defined(__ANDROID__)
	// This is an internal directory specifier pointing to the assets in the .apk
	// Files from this directory will be opened using special handling
	return APK;
#elif defined(IOS) || defined(TVOS)
	return AddTrailingSlash(SDL_IOS_GetResourceDir());
#elif defined(_WIN32)
	wchar_t exeName[MAX_PATH];
	exeName[0] = 0;
	GetModuleFileNameW(nullptr, exeName, MAX_PATH);
	return GetPath(STRING(exeName));
#elif defined(__APPLE__)
	char exeName[MAX_PATH];
	memset(exeName, 0, MAX_PATH);
	unsigned size = MAX_PATH;
	_NSGetExecutablePath(exeName, &size);
	return getPath(String(exeName));
#elif defined(__linux__)
	char exeName[MAX_PATH];
	memset(exeName, 0, MAX_PATH);
	pid_t pid = Getpid();
	String link = "/proc/" + String(pid) + "/exe";
	readlink(link.CString(), exeName, MAX_PATH);
	return GetPath(String(exeName));
#else
	return GetCurrentDir();
#endif
}

STRING FileSystem::GetCurrentDir()
{
#ifdef _WIN32
	wchar_t path[MAX_PATH];
	path[0] = 0;
	GetCurrentDirectoryW(MAX_PATH, path);
	return AddTrailingSlash(STRING(path));
#else
	char path[MAX_PATH];
	path[0] = 0;
	getcwd(path, MAX_PATH);
	return AddTrailingSlash(STRING(path));
#endif
}

STRING FileSystem::GetAppPreferencesDir(const STRING& org, const STRING& app)
{
	STRING dir;
#ifndef MINI_AUTO
	char* prefPath = SDL_GetPrefPath(org.CStr(), app.CStr());
	if (prefPath)
	{
		dir = GetInternalPath(STRING(prefPath));
		SDL_free(prefPath);
	}
	else
#endif
		LogString("Could not get application preferences directory");
	return dir;
}

STRING FileSystem::GetTemporaryDir()
{
#if defined(_WIN32)
#	if defined(MINI_AUTO)
		return getenv("TMP");
#	else
		wchar_t pathName[MAX_PATH];
		pathName[0] = 0;
		GetTempPathW(SDL_arraysize(pathName), pathName);
		return AddTrailingSlash(STRING(pathName));
#	endif
#else
	if (char* pathName = getenv("TMPDIR"))
		return AddTrailingSlash(pathName);
#	ifdef P_tmpdir
		return AddTrailingSlash(P_tmpdir);
#	else
		return "/tmp/";
#	endif
#endif
}

STRING FileSystem::AddTrailingSlash(const STRING& pathName)
{
	STRING ret = pathName;
	ret.Replace('\\', '/');
	if (!ret.Empty() && ret.Back() != '/')
		ret += '/';
	return ret;
}

STRING FileSystem::RemoveTrailingSlash(const STRING& pathName)
{
	STRING ret = pathName;
	ret.Replace('\\', '/');
	if (!ret.Empty() && ret.Back() == '/')
		ret.Resize(ret.Length() - 1);
	return ret;
}

bool FileSystem::IsAbsolutePath(const STRING& pathName)
{
	if (pathName.Empty())
		return false;

	STRING path = GetInternalPath(pathName);

	if (path[0] == '/')
		return true;

#ifdef _WIN32
	if (path.Length() > 1 && isalpha(path[0]) && path[1] == ':')
		return true;
#endif
	return false;
}

bool FileSystem::CheckAccess(const STRING& pathName)
{
	STRING fixedPath = AddTrailingSlash(pathName);

	// If no allowed directories defined, succeed always
	if (_allowedPaths.empty())
		return true;

	// If there is any attempt to go to a parent directory, disallow
	
	if (fixedPath.Contains(".."))
		return false;

	// Check if the path is a partial match of any of the allowed directories
	for (HASH_SET<STRING>::const_iterator i = _allowedPaths.begin(); i != _allowedPaths.end(); ++i)
	{
		if (fixedPath.Find(*i) == 0)
			return true;
	}

	// Not found, so disallow
	return false;
}

unsigned FileSystem::GetLastModifiedTime(const STRING& fileName)
{
	if (fileName.Empty() || !CheckAccess(fileName))
		return 0;

#ifdef _WIN32
	struct _stat st;
	if (!_stat(fileName.CStr(), &st))
		return (unsigned)st.st_mtime;
	else
		return 0;
#else
	struct stat st {};
	if (!stat(fileName.CString(), &st))
		return (unsigned)st.st_mtime;
	else
		return 0;
#endif
}

bool FileSystem::FileExists(const STRING& fileName)
{
	if (!CheckAccess(GetPath(fileName)))
		return false;

	STRING fixedName = GetNativePath(RemoveTrailingSlash(fileName));

#ifdef _WIN32
	DWORD attributes = GetFileAttributesW(WSTRING(fixedName).CStr());
	if (attributes == INVALID_FILE_ATTRIBUTES || attributes & FILE_ATTRIBUTE_DIRECTORY)
		return false;
#else
	struct stat st {};
	if (stat(fixedName.CStr(), &st) || st.st_mode & S_IFDIR)
		return false;
#endif

	return true;
}


bool FileSystem::DirExists(const STRING& pathName)
{
	if (!CheckAccess(pathName))
		return false;

#ifndef _WIN32
	// Always return true for the root directory
	if (pathName == "/")
		return true;
#endif

	STRING fixedName = GetNativePath(RemoveTrailingSlash(pathName));

#ifdef __ANDROID__
	if (AUTO_IS_ASSET(fixedName))
	{
		// Split the pathname into two components: the longest parent directory path and the last name component
		String assetPath(URHO3D_ASSET((fixedName + "/")));
		String parentPath;
		unsigned pos = assetPath.FindLast('/', assetPath.Length() - 2);
		if (pos != String::NPOS)
		{
			parentPath = assetPath.Substring(0, pos);
			assetPath = assetPath.Substring(pos + 1);
		}
		assetPath.Resize(assetPath.Length() - 1);

		bool exist = false;
		int count;
		char** list = SDL_Android_GetFileList(parentPath.CString(), &count);
		for (int i = 0; i < count; ++i)
		{
			exist = assetPath == list[i];
			if (exist)
				break;
		}
		SDL_Android_FreeFileList(&list, &count);
		return exist;
	}
#endif

#ifdef _WIN32
	DWORD attributes = GetFileAttributesW(WSTRING(fixedName).CStr());
	if (attributes == INVALID_FILE_ATTRIBUTES || !(attributes & FILE_ATTRIBUTE_DIRECTORY))
		return false;
#else
	struct stat st {};
	if (stat(fixedName.CStr(), &st) || !(st.st_mode & S_IFDIR))
		return false;
#endif

	return true;
}

///////////////////////////////////////////////////////////////////////////
// @brief : Global function
//////////////////////////////////////////////////////////////////////////

void SplitPath(const STRING& fullPath, STRING& pathName, STRING& fileName, STRING& extension, bool lowercaseExtension)
{
	STRING fullPathCopy = GetInternalPath(fullPath);
	unsigned extPos = fullPathCopy.FindLast('.');
	unsigned pathPos = fullPathCopy.FindLast('/');

	if (extPos != STRING::NO_POS && (pathPos == STRING::NO_POS || extPos > pathPos))
	{
		extension = fullPathCopy.SubString(extPos);
		if (lowercaseExtension)
			extension = extension.ToLower();
		fullPathCopy = fullPathCopy.SubString(0, extPos);
	}
	else
		extension.Clear();

	pathPos = fullPathCopy.FindLast('/');
	if (pathPos != STRING::NO_POS)
	{
		fileName = fullPathCopy.SubString(pathPos + 1);
		pathName = fullPathCopy.SubString(0, pathPos + 1);
	}
	else
	{
		fileName = fullPathCopy;
		pathName.Clear();
	}
}

STRING AddTrailingSlash(const STRING& pathName)
{
	STRING ret = pathName;
	ret.Replace('\\', '/');
	if (!ret.Empty() && ret.Back() != '/')
		ret += '/';
	return ret;
}

STRING RemoveTrailingSlash(const STRING& pathName)
{
	STRING ret = pathName;
	ret.Replace('\\', '/');
	if (!ret.Empty() && ret.Back() == '/')
		ret.Resize(ret.Length() - 1);
	return ret;
}

STRING GetParentPath(const STRING& path)
{
	unsigned pos = RemoveTrailingSlash(path).FindLast('/');
	if (pos != STRING::NO_POS)
		return path.SubString(0, pos + 1);
	else
		return STRING();
}

STRING GetPath(const STRING& fullPath)
{
	STRING path, file, extension;
	SplitPath(fullPath, path, file, extension);
	return path;
}

STRING GetFileName(const STRING& fullPath)
{
	STRING path, file, extension;
	SplitPath(fullPath, path, file, extension);
	return file;
}

STRING GetExtension(const STRING& fullPath, bool lowercaseExtension)
{
	STRING path, file, extension;
	SplitPath(fullPath, path, file, extension, lowercaseExtension);
	return extension;
}

STRING GetFileNameAndExtension(const STRING& fileName, bool lowercaseExtension)
{
	STRING path, file, extension;
	SplitPath(fileName, path, file, extension, lowercaseExtension);
	return file + extension;
}

STRING ReplaceExtension(const STRING& fullPath, const STRING& newExtension)
{
	STRING path, file, extension;
	SplitPath(fullPath, path, file, extension);
	return path + file + newExtension;
}

STRING GetInternalPath(const STRING& pathName)
{
	STRING ret = pathName;
	ret.Replace('\\', '/');
	return ret;
}

STRING GetNativePath(const STRING& pathName)
{
#ifdef _WIN32
	return pathName.Replaced('/', '\\');
#else
	return pathName;
#endif
}

WSTRING GetWideNativePath(const STRING& pathName)
{
#ifdef _WIN32
	return WSTRING(pathName.Replaced('/', '\\'));
#else
	return WSTRING(pathName);
#endif
}

bool IsAbsolutePath(const STRING& pathName)
{
	if (pathName.Empty())
		return false;

	STRING path = GetInternalPath(pathName);

	if (path[0] == '/')
		return true;

#ifdef _WIN32
	if (path.Length() > 1 && isalpha(path[0]) && path[1] == ':')
		return true;
#endif
	return false;
}


}