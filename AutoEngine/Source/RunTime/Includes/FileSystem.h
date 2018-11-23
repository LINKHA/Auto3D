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
	const WSTRING GetUserDocumentsDir();
	/**
	* @brief : Return the program's directory
	*/
	const WSTRING GetProgramDir();
	/**
	* @brief : Add trailing slash
	*/
	STRING AddTrailingSlash(const STRING& pathName);
	/**
	* @brief : Add trailing slash
	*/
	WSTRING AddTrailingSlash(const WSTRING& pathName);
private:
	/**
	* @brief : The internal limit of this template allows only STRING and WSTRING
	*/
	template<typename _Ty> void splitPath(const _Ty& fullPath, _Ty& pathName, _Ty& fileName, _Ty& extension, bool lowercaseExtension = true);
	
	STRING getPath(const STRING& fullPath);

	WSTRING getPath(const WSTRING& fullPath);


	STRING getInternalPath(const STRING& pathName);

	WSTRING getInternalPath(const WSTRING& pathName);

	STRING getNativePath(const STRING& pathName);

	WSTRING getNativePath(const WSTRING& pathName);
};

}

