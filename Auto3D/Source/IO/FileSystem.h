#pragma once
#include "Core/GameManager.h"
#include "Container/String.h"
#include "Container/Vector.h"
#include "Container/WString.h"
#include "Container/HashSet.h"

namespace Auto3D
{

/// Return files.
static const unsigned SCAN_FILES = 0x1;
/// Return directories.
static const unsigned SCAN_DIRS = 0x2;
/// Return also hidden files.
static const unsigned SCAN_HIDDEN = 0x4;

/// Module for file and directory operations and access control.
class AUTO_API AFileSystem : public ABaseModule
{
	REGISTER_OBJECT_CLASS(AFileSystem, ABaseModule)
public:
	/// Construct.
	AFileSystem();
	/// Destruct.
	~AFileSystem();
	/// Register a path as allowed to access. If no paths are registered,
	///	all are allowed. Registering allowed paths is considered securing the Auto3D
	///	execution environment: running programs and opening files externally through the system will fail afterward
	void RegisterPath(const FString& pathName);
	/// Set the current working directory.
	bool SetCurrentDir(const FString& pathName);
	/// Create a directory.
	bool CreateDir(const FString& pathName);
	/// Run a program using the command interpreter, block until it exits and return the exit code.Will fail if any allowed paths are defined
	int SystemCommand(const FString& commandLine, bool redirectStdOutToLog = false);
	/// Run a specific program, block until it exits and return the exit code.Will fail if any allowed paths are defined
	int SystemRun(const FString& fileName, const TVector<FString>& arguments);
	/// Copy a file. Return true on success.
	bool Copy(const FString& srcFileName, const FString& destFileName);
	/// Rename a file. Return true on success.
	bool Rename(const FString& srcFileName, const FString& destFileName);
	/// Delete a file. Return true on success.
	bool Delete(const FString& fileName);
	/// Return the absolute current working directory.
	FString GetCurrentDir();
private:
	/// Allowed directories
	THashSet<FString> _allowedPaths{};
};

/// Return the file's last modified time as seconds since epoch, or 0 if can not be accessed.
AUTO_API unsigned LastModifiedTime(const FString& fileName);
/// Set the file's last modified time as seconds since epoch. Return true on success.
AUTO_API bool SetLastModifiedTime(const FString& fileName, unsigned newTime);
/// Check if a file exists.
AUTO_API bool FileExists(const FString& fileName);
/// Check if a directory exists.
AUTO_API bool DirExists(const FString& pathName);
/// Scan a directory for specified files.
AUTO_API void ScanDir(TVector<FString>& result, const FString& pathName, const FString& filter, unsigned flags = SCAN_FILES, bool recursive = false);
/// Return the executable's directory.
AUTO_API FString ExecutableDir();
/// Split a full path to path, filename and extension. The extension will be converted to lowercase by default.
AUTO_API void SplitPath(const FString& fullPath, FString& pathName, FString& fileName, FString& extension, bool lowercaseExtension = true);
/// Return the path from a full path.
AUTO_API FString Path(const FString& fullPath);
/// Return the filename from a full path.
AUTO_API FString FileName(const FString& fullPath);
/// Return the extension from a full path, converted to lowercase by default.
AUTO_API FString Extension(const FString& fullPath, bool lowercaseExtension = true);
/// Return the filename and extension from a full path. The case of the extension is preserved by default, so that the file can be opened in case-sensitive operating systems.
AUTO_API FString FileNameAndExtension(const FString& fullPath, bool lowercaseExtension = false);
/// Replace the extension of a file name with another.
AUTO_API FString ReplaceExtension(const FString& fullPath, const FString& newExtension);
/// Add a slash at the end of the path if missing and convert to normalized format (use slashes.)
AUTO_API FString AddTrailingSlash(const FString& pathName);
/// Remove the slash from the end of a path if exists and convert to normalized format (use slashes.)
AUTO_API FString RemoveTrailingSlash(const FString& pathName);
/// Return the parent path, or the path itself if not available.
AUTO_API FString ParentPath(const FString& pathName);
/// Convert a path to normalized (internal) format which uses slashes.
AUTO_API FString NormalizePath(const FString& pathName);
/// Convert a path to the format required by the operating system.
AUTO_API FString NativePath(const FString& pathName);
/// Convert a path to the format required by the operating system in wide characters.
AUTO_API FWString WideNativePath(const FString& pathName);
/// Return whether a path is absolute.
AUTO_API bool IsAbsolutePath(const FString& pathName);

}
