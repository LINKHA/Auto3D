#pragma once
#include "GameManager.h"

namespace Auto3D {

class FileSystem : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(FileSystem, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(FileSystem);
public:
	explicit FileSystem(Ambient* ambient);
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

	STRING GetPath(const STRING& fullPath);

	WSTRING GetPath(const WSTRING& fullPath);

	STRING GetFileName(const STRING& fullPath);

	WSTRING GetFileName(const WSTRING& fullPath);

	STRING GetExtension(const STRING& fullPath, bool lowercaseExtension);

	WSTRING GetExtension(const WSTRING& fullPath, bool lowercaseExtension);

	STRING GetFileNameAndExtension(const STRING& fileName, bool lowercaseExtension);

	WSTRING GetFileNameAndExtension(const WSTRING& fileName, bool lowercaseExtension);

	STRING ReplaceExtension(const STRING& fullPath, const STRING& newExtension);

	WSTRING ReplaceExtension(const WSTRING& fullPath, const WSTRING& newExtension);
	/**
	* @brief : Add trailing slash
	*/
	STRING AddTrailingSlash(const STRING& pathName);
	/**
	* @brief : Add trailing slash
	*/
	WSTRING AddTrailingSlash(const WSTRING& pathName);

	STRING GetInternalPath(const STRING& pathName);

	WSTRING GetInternalPath(const WSTRING& pathName);

	STRING GetNativePath(const STRING& pathName);

	WSTRING GetNativePath(const WSTRING& pathName);
	/**
	* @brief : Return whether a path is absolute
	*/
	bool IsAbsolutePath(const STRING& pathName);
	/**
	* @brief : Return whether a path is absolute with wstring
	*/
	bool IsAbsolutePath(const WSTRING& pathName);
private:
	/**
	* @brief : The internal limit of this template allows only STRING and WSTRING
	*/
	template<typename _Ty> void splitPath(const _Ty& fullPath, _Ty& pathName, _Ty& fileName, _Ty& extension, bool lowercaseExtension = true);
private:

};

}

