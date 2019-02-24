#pragma once

#include "../Base/String.h"
#include "../Base/Vector.h"
#include "../Base/WString.h"

namespace Auto3D
{

/// Return files.
static const unsigned SCAN_FILES = 0x1;
/// Return directories.
static const unsigned SCAN_DIRS = 0x2;
/// Return also hidden files.
static const unsigned SCAN_HIDDEN = 0x4;

/// Set the current working directory.
AUTO_API bool SetCurrentDir(const String& pathName);
/// Create a directory.
AUTO_API bool CreateDir(const String& pathName);
/// Copy a file. Return true on success.
AUTO_API bool CopyFile(const String& srcFileName, const String& destFileName);
/// Rename a file. Return true on success.
AUTO_API bool RenameFile(const String& srcFileName, const String& destFileName);
/// Delete a file. Return true on success.
AUTO_API bool DeleteFile(const String& fileName);
/// Return the absolute current working directory.
AUTO_API String CurrentDir();
/// Return the file's last modified time as seconds since epoch, or 0 if can not be accessed.
AUTO_API unsigned LastModifiedTime(const String& fileName);
/// Set the file's last modified time as seconds since epoch. Return true on success.
AUTO_API bool SetLastModifiedTime(const String& fileName, unsigned newTime);
/// Check if a file exists.
AUTO_API bool FileExists(const String& fileName);
/// Check if a directory exists.
AUTO_API bool DirExists(const String& pathName);
/// Scan a directory for specified files.
AUTO_API void ScanDir(Vector<String>& result, const String& pathName, const String& filter, unsigned flags = SCAN_FILES, bool recursive = false);
/// Return the executable's directory.
AUTO_API String ExecutableDir();
/// Split a full path to path, filename and extension. The extension will be converted to lowercase by default.
AUTO_API void SplitPath(const String& fullPath, String& pathName, String& fileName, String& extension, bool lowercaseExtension = true);
/// Return the path from a full path.
AUTO_API String Path(const String& fullPath);
/// Return the filename from a full path.
AUTO_API String FileName(const String& fullPath);
/// Return the extension from a full path, converted to lowercase by default.
AUTO_API String Extension(const String& fullPath, bool lowercaseExtension = true);
/// Return the filename and extension from a full path. The case of the extension is preserved by default, so that the file can be opened in case-sensitive operating systems.
AUTO_API String FileNameAndExtension(const String& fullPath, bool lowercaseExtension = false);
/// Replace the extension of a file name with another.
AUTO_API String ReplaceExtension(const String& fullPath, const String& newExtension);
/// Add a slash at the end of the path if missing and convert to normalized format (use slashes.)
AUTO_API String AddTrailingSlash(const String& pathName);
/// Remove the slash from the end of a path if exists and convert to normalized format (use slashes.)
AUTO_API String RemoveTrailingSlash(const String& pathName);
/// Return the parent path, or the path itself if not available.
AUTO_API String ParentPath(const String& pathName);
/// Convert a path to normalized (internal) format which uses slashes.
AUTO_API String NormalizePath(const String& pathName);
/// Convert a path to the format required by the operating system.
AUTO_API String NativePath(const String& pathName);
/// Convert a path to the format required by the operating system in wide characters.
AUTO_API WString WideNativePath(const String& pathName);
/// Return whether a path is absolute.
AUTO_API bool IsAbsolutePath(const String& pathName);

}
