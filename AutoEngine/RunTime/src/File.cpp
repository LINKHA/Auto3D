#include "File.h"
#include "LogAssert.h"
namespace Auto3D {
HANDLE OpenFileWithPath(const __String& path, File::Permission permission)
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


File::File() { _file = NULL; _position = 0; }

File::~File() { Assert(_file == NULL); }

bool File::Open(const std::string & path, Permission perm, ATBehavior behavior)
{
	Close();
	_path = path;
	int retryCount = 5;
	while (true)
	{

	}
	return false;
}
bool File::Close()
{
	if (_file != NULL)
	{
		if (fclose(_file) != 0)
		{

		}
		_file = NULL;
	}
	_path.clear();
	return true;
}

}