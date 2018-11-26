#include "File.h"
#include "LogAssert.h"
#include "FileSystem.h"
#include "Math/MathBase.h"

namespace Auto3D {

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable:4996)
#endif

#ifdef _WIN32
static const wchar_t* openMode[] =
{
	L"rb",
	L"wb",
	L"r+b",
	L"w+b"
};
#else
static const char* openMode[] =
{
	"rb",
	"wb",
	"r+b",
	"w+b"
};
#endif

File::File(Ambient* ambient)
	: Super(ambient)
	, _mode(FileMode::Read)
	, _handle(nullptr)
	, _readBufferOffset(0)
	, _readBufferSize(0)
	, _offset(0)
	, _checksum(0)
	, _compressed(false)
	, _readSyncNeeded(false)
	, _writeSyncNeeded(false)
{}

File::File(Ambient* ambient, const STRING& fileName, FileMode mode)
	: Super(ambient)
	, _mode(FileMode::Read)
	, _handle(nullptr)
	, _readBufferOffset(0)
	, _readBufferSize(0)
	, _offset(0)
	, _checksum(0)
	, _compressed(false)
	, _readSyncNeeded(false)
	, _writeSyncNeeded(false)
{
	Open(fileName, mode);
}

File::~File() 
{
}

unsigned File::Read(void* dest, unsigned size)
{
	return 0;
}

unsigned File::Seek(unsigned position)
{
	return 0;
}

unsigned File::Write(const void* data, unsigned size)
{
	return 0;
}

bool File::Open(const STRING& fileName, FileMode mode)
{
#ifdef _WIN32
	return openInternal(StringToWString(fileName), mode);
#else
	return openInternal(fileName, mode);
#endif
}

void File::Close()
{
	if (_handle)
	{
		fclose((FILE*)_handle);
		_handle = nullptr;
		_position = 0;
		_size = 0;
		_offset = 0;
		_checksum = 0;
	}
}

void File::Flush()
{
	if (_handle)
		fflush((FILE*)_handle);
}
void File::SetName(const STRING& name)
{
#ifdef _WIN32
	_fileName = StringToWString(name);
#else
	_fileName = name;
#endif
}

bool File::IsOpen() const
{
	return _handle != nullptr;
}

template <typename _Ty> bool File::openInternal(const _Ty& fileName, FileMode mode, bool fromPackage)
{
	Close();

	_compressed = false;
	_readSyncNeeded = false;
	_writeSyncNeeded = false;
	auto* fileSystem = GetSubSystem<FileSystem>();

	if (fileSystem && !fileSystem->CheckAccess(GetPath(fileName)))
	{
#ifdef _WIN32
		AutoWCout << L"Access denied to" << fileName << AutoEndl;
#else
		AutoCout << L"Access denied to" << fileName << AutoEndl;
#endif
		return false;
	}

	if (fileName.empty())
	{
		LogString("Could not open file with empty name");
		return false;
	}
#ifdef _WIN32
	_handle = _wfopen(GetNativePath(fileName).c_str(), openMode[static_cast<int>(mode)]);
#else
	_handle = fopen(GetNativePath(fileName).c_str(), openMode[static_cast<int>(mode)]);
#endif
	// If file did not exist in readwrite mode, retry with write-update mode
	if (mode == FileMode::ReadWrite && !_handle)
	{
#ifdef _WIN32
		_handle = _wfopen(GetNativePath(fileName).c_str(), openMode[static_cast<int>(mode) + 1]);
#else
		_handle = fopen(GetNativePath(fileName).c_str(), openMode[static_cast<int>(mode) + 1]);
#endif
	}

	if (!_handle)
	{
#ifdef _WIN32
		AutoWCout << "Could not open file" <<  fileName << AutoEndl;
#else
		AutoCout << "Could not open file" << fileName << AutoEndl;
#endif
		return false;
	}

	if (!fromPackage)
	{
		fseek((FILE*)_handle, 0, SEEK_END);
		long size = ftell((FILE*)_handle);
		fseek((FILE*)_handle, 0, SEEK_SET);
		if (size > MATH_MAX_UNSIGNED)
		{
#ifdef _WIN32
			AutoWCout << "Could not open file" << fileName<< "which is larger than 4GB"  << AutoEndl;
#else
			AutoCout << "Could not open file" << fileName << "which is larger than 4GB" << AutoEndl;
#endif
			Close();
			_size = 0;
			return false;
		}
		_size = (unsigned)size;
		_offset = 0;
	}

	_fileName = fileName;
	_mode = mode;
	_position = 0;
	_checksum = 0;

	return true;
}

}