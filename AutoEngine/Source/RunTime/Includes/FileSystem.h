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
	void RegisterPath(const STRING& pathName); void RegisterPath(const WSTRING& pathName);
	/**
	* @brief : Return the user documents directory
	*/
	WSTRING GetUserDocumentsDir();
	/**
	* @brief : Return the program's directory
	*/
	WSTRING GetProgramDir();
	/**
	* @brief : Return the absolute current working directory
	*/
	WSTRING GetCurrentDir();
	/**
	* @brief : Return the application preferences directory
	*/
	STRING GetAppPreferencesDir(const STRING& org, const STRING& app);
	/**
	* @brief : Return path of temporary directory. Path always ends with a forward slash
	*/
	WSTRING GetTemporaryDir();

	STRING GetPath(const STRING& fullPath);	WSTRING GetPath(const WSTRING& fullPath);

	STRING GetFileName(const STRING& fullPath); WSTRING GetFileName(const WSTRING& fullPath);

	STRING GetExtension(const STRING& fullPath, bool lowercaseExtension); WSTRING GetExtension(const WSTRING& fullPath, bool lowercaseExtension);

	STRING GetFileNameAndExtension(const STRING& fileName, bool lowercaseExtension); WSTRING GetFileNameAndExtension(const WSTRING& fileName, bool lowercaseExtension);

	STRING ReplaceExtension(const STRING& fullPath, const STRING& newExtension); WSTRING ReplaceExtension(const WSTRING& fullPath, const WSTRING& newExtension);
	/**
	* @brief : Add trailing slash
	*/
	STRING AddTrailingSlash(const STRING& pathName); WSTRING AddTrailingSlash(const WSTRING& pathName);
	/**
	* @brief : Remove trailing slash
	*/
	STRING RemoveTrailingSlash(const STRING& pathName); WSTRING RemoveTrailingSlash(const WSTRING& pathName);

	STRING GetParentPath(const STRING& path); WSTRING GetParentPath(const WSTRING& path);

	STRING GetInternalPath(const STRING& pathName); WSTRING GetInternalPath(const WSTRING& pathName);

	STRING GetNativePath(const STRING& pathName); WSTRING GetNativePath(const WSTRING& pathName);
	/**
	* @brief : Return whether a path is absolute
	*/
	bool IsAbsolutePath(const STRING& pathName); bool IsAbsolutePath(const WSTRING& pathName);
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
	template<typename _Ty> void SplitPath(const _Ty& fullPath, _Ty& pathName, _Ty& fileName, _Ty& extension, bool lowercaseExtension = true);
private:
	/// Allowed directories
	HASH_SET<STRING> _allowedPaths;
	/// Allowed directories
	HASH_SET<WSTRING> _allowedWPaths;
};

/**
* @brief : The internal limit of this template allows only STRING and WSTRING
*/
template<typename _Ty> void SplitPath(const _Ty& fullPath, _Ty& pathName, _Ty& fileName, _Ty& extension, bool lowercaseExtension = true);

STRING GetPath(const STRING& fullPath);	WSTRING GetPath(const WSTRING& fullPath);

STRING GetFileName(const STRING& fullPath); WSTRING GetFileName(const WSTRING& fullPath);

STRING GetExtension(const STRING& fullPath, bool lowercaseExtension); WSTRING GetExtension(const WSTRING& fullPath, bool lowercaseExtension);

STRING GetFileNameAndExtension(const STRING& fileName, bool lowercaseExtension); WSTRING GetFileNameAndExtension(const WSTRING& fileName, bool lowercaseExtension);

STRING ReplaceExtension(const STRING& fullPath, const STRING& newExtension); WSTRING ReplaceExtension(const WSTRING& fullPath, const WSTRING& newExtension);
/**
* @brief : Add trailing slash
*/
STRING AddTrailingSlash(const STRING& pathName); WSTRING AddTrailingSlash(const WSTRING& pathName);
/**
* @brief : Remove trailing slash
*/
STRING RemoveTrailingSlash(const STRING& pathName); WSTRING RemoveTrailingSlash(const WSTRING& pathName);

STRING GetParentPath(const STRING& path); WSTRING GetParentPath(const WSTRING& path);

STRING GetInternalPath(const STRING& pathName); WSTRING GetInternalPath(const WSTRING& pathName);

STRING GetNativePath(const STRING& pathName); WSTRING GetNativePath(const WSTRING& pathName);
/**
* @brief : Return whether a path is absolute
*/
bool IsAbsolutePath(const STRING& pathName); bool IsAbsolutePath(const WSTRING& pathName);

}

