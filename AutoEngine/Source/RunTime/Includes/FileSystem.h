#pragma once
#include "GameManager.h"
#include "AutoSTL.h"
#include <memory>

namespace Auto3D {

class FileSystem : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(FileSystem, GlobalGameManager)
public:
	explicit FileSystem(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register a path as allowed to access. If no paths are registered,
	*	all are allowed. Registering allowed paths is considered securing the Auto3D 
	*	execution environment: running programs and opening files externally through the system will fail afterward
	*/
	void RegisterPath(const STRING& pathName);
	/**
	* @brief : Set the current working directory
	*/
	bool SetCurrentDir(const STRING& pathName);
	/**
	* @brief : Create a directory
	*/
	bool CreateDir(const STRING& pathName);
	/**
	* @brief :Run a program using the command interpreter, block until it exits and return the exit code.
	*		Will fail if any allowed paths are defined
	*/
	int SystemCommand(const STRING& commandLine, bool redirectStdOutToLog = false);
	/**
	* @brief : Run a specific program, block until it exits and return the exit code
	*		Will fail if any allowed paths are defined
	*/
	int SystemRun(const STRING& fileName, const VECTOR<STRING>& arguments);

	/**
	* @brief : Copy a file. Return true if successful
	*/
	bool Copy(const STRING& srcFileName, const STRING& destFileName);
	/**
	* @brief : Rename a file. Return true if successful
	*/
	bool Rename(const STRING& srcFileName, const STRING& destFileName);
	/**
	* @brief :  Delete a file. Return true if successful
	*/
	bool Delete(const STRING& fileName);


	/**
	* @brief : Return the user documents directory
	*/
	STRING GetUserDocumentsDir();
	/**
	* @brief : Return the program's directory
	*/
	STRING GetProgramDir();
	/**
	* @brief : Return the absolute current working directory
	*/
	STRING GetCurrentDir();
	/**
	* @brief : Return the application preferences directory
	*/
	STRING GetAppPreferencesDir(const STRING& org, const STRING& app);
	/**
	* @brief : Return path of temporary directory. Path always ends with a forward slash
	*/
	STRING GetTemporaryDir();
	/**
	* @brief : Add trailing slash
	*/
	STRING AddTrailingSlash(const STRING& pathName);
	/**
	* @brief : Remove trailing slash
	*/
	STRING RemoveTrailingSlash(const STRING& pathName);
	/**
	* @brief : Return whether a path is absolute
	*/
	bool IsAbsolutePath(const STRING& pathName);
	/**
	* @brief : Check if a path is allowed to be accessed. 
	*	If no paths are registered, all are allowed
	*/
	bool CheckAccess(const STRING& pathName);
	/** 
	* @brief : Returns the file's last modified time as seconds since 1.1.1970, or 0 if can not be accessed
	*/
	unsigned GetLastModifiedTime(const STRING& fileName);
	/**
	* @brief : Check if a file exists
	*/
	bool FileExists(const STRING& fileName);
	/**
	* @brief : Check if a directory exists
	*/
	bool DirExists(const STRING& pathName);
private:
	/// Allowed directories
	HASH_SET<STRING> _allowedPaths{};
};

/**
* @brief : The internal limit of this template allows only STRING and WSTRING
*/
void SplitPath(const STRING& fullPath, STRING& pathName, STRING& fileName, STRING& extension, bool lowercaseExtension = true);

STRING GetPath(const STRING& fullPath);

STRING GetFileName(const STRING& fullPath);

STRING GetExtension(const STRING& fullPath, bool lowercaseExtension = true);

STRING GetFileNameAndExtension(const STRING& fileName, bool lowercaseExtension = false);

STRING ReplaceExtension(const STRING& fullPath, const STRING& newExtension);
/**
* @brief : Add trailing slash
*/
STRING AddTrailingSlash(const STRING& pathName);
/**
* @brief : Remove trailing slash
*/
STRING RemoveTrailingSlash(const STRING& pathName);

STRING GetParentPath(const STRING& path);

STRING GetInternalPath(const STRING& pathName);

STRING GetNativePath(const STRING& pathName);

WSTRING GetWideNativePath(const STRING& pathName);
/**
* @brief : Return whether a path is absolute
*/
bool IsAbsolutePath(const STRING& pathName);

}

