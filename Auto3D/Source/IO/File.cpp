#include "File.h"
#include "FileSystem.h"

#include <cstdio>

#include "../Debug/DebugNew.h"

namespace Auto3D
{

#ifdef _WIN32
static const wchar_t* openModes[] =
{
    L"rb",
    L"wb",
    L"r+b",
    L"w+b"
};
#else
static const char* openModes[] =
{
    "rb",
    "wb",
    "r+b",
    "w+b"
};
#endif

File::File() :
    _mode(FileMode::READ),
    _handle(nullptr),
    _readSyncNeeded(false),
    _writeSyncNeeded(false)
{
}

File::File(const String& fileName, FileMode::Type mode) :
    _mode(FileMode::READ),
    _handle(nullptr),
    _readSyncNeeded(false),
    _writeSyncNeeded(false)
{
    Open(fileName, mode);
}

File::~File()
{
    Close();
}

bool File::Open(const String& fileName, FileMode::Type fileMode)
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
    if (_mode == FileMode::READWRITE && !_handle)
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

size_t File::Read(void* dest, size_t numBytes)
{
    if (!_handle || _mode == FileMode::WRITE)
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

size_t File::Seek(size_t newPosition)
{
    if (!_handle)
        return 0;
    
    // Allow sparse seeks if writing
    if (_mode == FileMode::READ && newPosition > _size)
        newPosition = _size;

    fseek((FILE*)_handle, (long)newPosition, SEEK_SET);
    _position = newPosition;
    _readSyncNeeded = false;
    _writeSyncNeeded = false;
    return _position;
}

size_t File::Write(const void* _data, size_t numBytes)
{
    if (!_handle || _mode == FileMode::READ)
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

bool File::IsReadable() const
{
    return _handle != 0 && _mode != FileMode::WRITE;
}

bool File::IsWritable() const
{
    return _handle != 0 && _mode != FileMode::READ;
}

void File::Close()
{
    if (_handle)
    {
        fclose((FILE*)_handle);
        _handle = 0;
        _position = 0;
        _size = 0;
    }
}

void File::Flush()
{
    if (_handle)
        fflush((FILE*)_handle);
}

bool File::IsOpen() const
{
    return _handle != 0;
}

}
