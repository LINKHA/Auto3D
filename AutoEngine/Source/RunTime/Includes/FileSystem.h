#pragma once
#include "GameManager.h"
#include "AutoSTL.h"
#include <memory>

#ifdef _WIN32
#	define PATH_STRING WSTRING
#else
#	define PATH_STRING STRING
#endif

namespace Auto3D {

class FileSystem : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(FileSystem, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(FileSystem);
public:
	explicit FileSystem(Ambient* ambient);
	/**
	* @brief : Register a path as allowed to access. If no paths are registered,
	*	all are allowed. Registering allowed paths is considered securing the Auto3D 
	*	execution environment: running programs and opening files externally through the system will fail afterward
	*/
	void RegisterPath(const STRING& pathName);
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

	STRING GetPath(const STRING& fullPath);

	STRING GetFileName(const STRING& fullPath);

	STRING GetExtension(const STRING& fullPath, bool lowercaseExtension);

	STRING GetFileNameAndExtension(const STRING& fileName, bool lowercaseExtension);

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
	/**
	* @brief : Return whether a path is absolute
	*/
	bool IsAbsolutePath(const STRING& pathName);
	/**
	* @brief : Check if a path is allowed to be accessed. 
	*	If no paths are registered, all are allowed
	*/
	bool CheckAccess(const STRING& pathName); bool CheckAccess(const WSTRING& pathName);
	/**
	* @brief : Check if a file exists
	*/
	bool FileExists(const STRING& fileName);
	/**
	* @brief : The internal limit of this template allows only STRING and WSTRING
	*/
	void SplitPath(const STRING& fullPath, STRING& pathName, STRING& fileName, STRING& extension, bool lowercaseExtension = true);
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

STRING GetExtension(const STRING& fullPath, bool lowercaseExtension);

STRING GetFileNameAndExtension(const STRING& fileName, bool lowercaseExtension);

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
/**
* @brief : Return whether a path is absolute
*/
bool IsAbsolutePath(const STRING& pathName);

}

