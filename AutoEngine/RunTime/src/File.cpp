#include "File.h"


HANDLE OpenFileWithPath(const _String& path, File::Permission permission)
{
	wchar_t widePath[kPathMaxSize];
	//ConvertPathName(path.c_str(), widePath, kPathMaxSize);

	DWORD accessMode, shareMode, createMode;
	switch (permission) {
	case File::kReadPermission:
		accessMode = FILE_GENERIC_READ;
		shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
		createMode = OPEN_EXISTING;
		break;
	case File::kWritePermission:
		accessMode = FILE_GENERIC_WRITE;
		shareMode = 0;
		createMode = CREATE_ALWAYS;
		break;
	case File::kAppendPermission:
		accessMode = FILE_GENERIC_WRITE;
		shareMode = 0;
		createMode = OPEN_ALWAYS;
		break;
	case File::kReadWritePermission:
		accessMode = FILE_GENERIC_READ | FILE_GENERIC_WRITE;
		shareMode = 0;
		createMode = OPEN_ALWAYS;
		break;
	default:
		//AssertString("Unknown permission mode");
		return INVALID_HANDLE_VALUE;
	}
	HANDLE fileHandle = CreateFileW(widePath, accessMode, shareMode, NULL, createMode, NULL, NULL);
	if (permission == File::kAppendPermission && fileHandle != INVALID_HANDLE_VALUE)
		SetFilePointer(fileHandle, 0, NULL, FILE_END);
	return fileHandle;
}


File::File() { m_File = NULL; m_Position = 0; }

File::~File() { AssertIf(m_File != NULL); }

bool File::Open(const std::string & path, Permission perm, ATBehavior behavior)
{
	Close();
	m_Path = path;
	int retryCount = 5;
	while (true)
	{

	}
	return false;
}
