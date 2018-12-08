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

STRING FileSystem::GetParentPath(const STRING& path)
{
	unsigned pos = RemoveTrailingSlash(path).FindLast('/');
	if (pos != STRING::NO_POS)
		return path.SubString(0, pos + 1);
	else
		return STRING();
}

STRING FileSystem::GetPath(const STRING& fullPath)
{
	STRING path, file, extension;
	SplitPath(fullPath, path, file, extension);
	return path;
}

STRING FileSystem::GetFileName(const STRING& fullPath)
{
	STRING path, file, extension;
	SplitPath(fullPath, path, file, extension);
	return file;
}

STRING FileSystem::GetExtension(const STRING& fullPath, bool lowercaseExtension)
{
	STRING path, file, extension;
	SplitPath(fullPath, path, file, extension, lowercaseExtension);
	return extension;
}

STRING FileSystem::GetFileNameAndExtension(const STRING& fileName, bool lowercaseExtension)
{
	STRING path, file, extension;
	SplitPath(fileName, path, file, extension, lowercaseExtension);
	return file + extension;
}

STRING FileSystem::ReplaceExtension(const STRING& fullPath, const STRING& newExtension)
{
	STRING path, file, extension;
	SplitPath(fullPath, path, file, extension);
	return path + file + newExtension;
}

STRING FileSystem::GetInternalPath(const STRING& pathName)
{
	STRING ret = pathName;
	ret.Replace('\\', '/');
	return ret;
}

STRING FileSystem::GetNativePath(const STRING& pathName)
{
	STRING ret = pathName;
	ret.Replace('/', '\\');
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

void FileSystem::SplitPath(const STRING& fullPath, STRING& pathName, STRING& fileName, STRING& extension, bool lowercaseExtension)
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
	STRING ret = pathName;
	ret.Replace('/', '\\');
	return ret;
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