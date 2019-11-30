#include "File.h"
#include "FileSystem.h"

#include <cstdio>

#include "Debug/DebugNew.h"

namespace Auto3D
{

#ifdef _WIN32
static const wchar_t* openModes[] =
{
    L"rb",
    L"wb",
    L"r+b",
    L"w+b",
	L"rt",
	L"wt",
	L"r+t",
	L"w+t"
};
#else
static const char* openModes[] =
{
    "rb",
    "wb",
    "r+b",
    "w+b",
	"rt",
	"wt",
	"r+t",
	"w+t"
};
#endif

FFile::FFile() :
    _mode(EFileMode::READ),
    _handle(nullptr),
    _readSyncNeeded(false),
    _writeSyncNeeded(false)
{
}

FFile::FFile(const FString& fileName, EFileMode::Type mode) :
    _mode(EFileMode::READ),
    _handle(nullptr),
    _readSyncNeeded(false),
    _writeSyncNeeded(false)
{
    Open(fileName, mode);
}

FFile::~FFile()
{
    Close();
}

bool FFile::Open(const FString& fileName, EFileMode::Type fileMode)
{
    Close();

    if (fileName.IsEmpty())
        return false;
    
    #ifdef _WIN32
    _handle = _wfopen(WideNativePath(fileName).CString(), openModes[fileMode]);
    #else
    _handle = fopen(NativePath(fileName).CString(), openModes[fileMode]);
    #endif

    // If file did not exist in readwrite mode, retry with write-update mode
    if (_mode == EFileMode::READ_WRITE && !_handle)
    {
        #ifdef _WIN32
        _handle = _wfopen(WideNativePath(fileName).CString(), openModes[fileMode + 1]);
        #else
        _handle = fopen(NativePath(fileName).CString(), openModes[fileMode + 1]);
        #endif
    }
    
    if (!_handle)
        return false;

    _name = fileName;
    _mode = fileMode;
    _position = 0;
    _readSyncNeeded = false;
    _writeSyncNeeded = false;

    fseek((FILE*)_handle, 0, SEEK_END);
    _size = ftell((FILE*)_handle);
    fseek((FILE*)_handle, 0, SEEK_SET);
    return true;
}

size_t FFile::Read(void* dest, size_t numBytes)
{
    if (!_handle || _mode == EFileMode::WRITE)
        return 0;

    if (numBytes + _position > _size)
        numBytes = _size - _position;
    if (!numBytes)
        return 0;

    // Need to reassign the _position due to internal buffering when transitioning from writing to reading
    if (_readSyncNeeded)
    {
        fseek((FILE*)_handle, (long)_position, SEEK_SET);
        _readSyncNeeded = false;
    }
    
    size_t ret = fread(dest, numBytes, 1, (FILE*)_handle);
    if (ret != 1)
    {
        // If error, return to the _position where the read began
        fseek((FILE*)_handle, (long)_position, SEEK_SET);
        return 0;
    }

    _writeSyncNeeded = true;
    _position += numBytes;
    return numBytes;
}

size_t FFile::Seek(size_t newPosition)
{
    if (!_handle)
        return 0;
    
    // Allow sparse seeks if writing
    if (_mode == EFileMode::READ && newPosition > _size)
        newPosition = _size;

    fseek((FILE*)_handle, (long)newPosition, SEEK_SET);
    _position = newPosition;
    _readSyncNeeded = false;
    _writeSyncNeeded = false;
    return _position;
}

size_t FFile::Write(const void* _data, size_t numBytes)
{
    if (!_handle || _mode == EFileMode::READ)
        return 0;

    if (!numBytes)
        return 0;

    // Need to reassign the _position due to internal buffering when transitioning from reading to writing
    if (_writeSyncNeeded)
    {
        fseek((FILE*)_handle, (long)_position, SEEK_SET);
        _writeSyncNeeded = false;
    }
    
    if (fwrite(_data, numBytes, 1, (FILE*)_handle) != 1)
    {
        // If error, return to the _position where the write began
        fseek((FILE*)_handle, (long)_position, SEEK_SET);
        return 0;
    }

    _readSyncNeeded = true;
    _position += numBytes;
    if (_position > _size)
        _size = _position;

    return _size;
}

bool FFile::IsReadable() const
{
    return _handle != 0 && _mode != EFileMode::WRITE;
}

bool FFile::IsWritable() const
{
    return _handle != 0 && _mode != EFileMode::READ;
}

void FFile::Close()
{
    if (_handle)
    {
        fclose((FILE*)_handle);
        _handle = 0;
        _position = 0;
        _size = 0;
    }
}

void FFile::Flush()
{
    if (_handle)
        fflush((FILE*)_handle);
}

bool FFile::IsOpen() const
{
    return _handle != 0;
}

}
