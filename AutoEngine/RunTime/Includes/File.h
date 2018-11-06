#pragma once
#include <windows.h>
#include "Auto.h"
#include "AutoSTL.h"
#include "LogAssert.h"
namespace Auto3D {
#define kPathMaxSize MAX_PATH * 4


class File
{
	int							_position;
	__String						_path;
	bool						_open;
	FILE*						_file;
	HANDLE						_fileHandle;
	VECTOR(unsigned char)		_data;

public:
	File();
	virtual ~File();

	enum Permission { kReadPermission = 0, kWritePermission = 1, kReadWritePermission = 2, kAppendPermission = 3 };
	enum ATBehavior { kNormalBehavior = 0, kSilentReturnOnOpenFail = 1 << 0, kRetryOnOpenFail = 1 << 1 };

	bool Open(const std::string& path, Permission perm, ATBehavior behavior = kNormalBehavior);
	bool Close();

	int Read(void* buffer, int size);
	int Read(int position, void* buffer, int size);

	bool Write(const void* buffer, int size);
	bool Write(int pos, const void* buffer, int size);
	bool SetFileLength(int size);
	int GetFileLength();
	int GetPosition() const { return _position; }

	static void SetCurrentDirectory(const std::string & path);
	static const __String& GetCurrentDirectory();
	static void CleanUpClass();

};

}