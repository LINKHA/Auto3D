#pragma once

#include "Stream.h"

namespace Auto3D
{

/// %FFile open mode.
namespace EFileMode
{
	enum Type
	{
		READ = 0,
		WRITE,
		READ_WRITE,
		READ_WRITE_CLEAR,
		READ_TXT,
		WRITE_TXT,
		READ_WRITE_TXT,
		READ_WRITE_CLEAR_TXT
	};
};

/// Filesystem file.
class AUTO_API FFile : public FStream
{
public:
    /// Construct.
    FFile();
    /// Construct and open a file.
    FFile(const FString& fileName, EFileMode::Type fileMode = EFileMode::READ);
    /// Destruct. Close the file if open.
    ~FFile();
    
    /// Read bytes from the file. Return number of bytes actually read.
    size_t Read(void* dest, size_t numBytes) override;
    /// Set _position in bytes from the beginning of the file.
    size_t Seek(size_t newPosition) override;
    /// Write bytes to the file. Return number of bytes actually written.
    size_t Write(const void* _data, size_t numBytes) override;
    /// Return whether read operations are allowed.
    bool IsReadable() const override;
    /// Return whether write operations are allowed.
    bool IsWritable() const override;

    /// Open a file. Return true on success.
    bool Open(const FString& fileName, EFileMode::Type fileMode = EFileMode::READ);
    /// Close the file.
    void Close();
    /// Flush any buffered output to the file.
    void Flush();
    
    /// Return the open mode.
    EFileMode::Type Mode() const { return _mode; }
    /// Return whether is open.
    bool IsOpen() const;
    /// Return the file _handle.
    void* Handle() const { return _handle; }
    
    using FStream::Read;
    using FStream::Write;
    
private:
    /// Open mode.
    EFileMode::Type _mode;
    /// FFile _handle.
    void* _handle;
    /// Synchronization needed before read -flag.
    bool _readSyncNeeded;
    /// Synchronization needed before write -flag.
    bool _writeSyncNeeded;
};

}
