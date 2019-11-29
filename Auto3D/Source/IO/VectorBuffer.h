#pragma once

#include "Container/Vector.h"
#include "Stream.h"

namespace Auto3D
{

/// Dynamically sized buffer that can be read and written to as a stream.
class AUTO_API VectorBuffer : public Stream
{
public:
    /// Construct an empty buffer.
    VectorBuffer();
    /// Construct from another buffer.
    VectorBuffer(const TVector<unsigned char>& _data);
    /// Construct from a memory area.
    VectorBuffer(const void* _data, size_t numBytes);
    /// Construct from a stream.
    VectorBuffer(Stream& source, size_t numBytes);
    
    /// Read bytes from the buffer. Return number of bytes actually read.
    size_t Read(void* dest, size_t _size) override;
    /// Set _position in bytes from the beginning of the buffer.
    size_t Seek(size_t newPosition) override;
    /// Write bytes to the buffer. Return number of bytes actually written.
    size_t Write(const void* _data, size_t _size) override;
    /// Return whether read operations are allowed.
    bool IsReadable() const override;
    /// Return whether write operations are allowed.
    bool IsWritable() const override;

    /// Set data from another buffer.
    void SetData(const TVector<unsigned char>& _data);
    /// Set data from a memory area.
    void SetData(const void* _data, size_t numBytes);
    /// Set data from a stream.
    void SetData(Stream& source, size_t numBytes);
    /// Reset to zero _size.
    void Clear();
    /// Set _size.
    void Resize(size_t newSize);
    
    /// Return data.
    const unsigned char* Data() const { return _buffer.Begin()._ptr; }
    /// Return non-const data.
    unsigned char* ModifiableData() { return _buffer.Begin()._ptr; }
    /// Return the buffer.
    const TVector<unsigned char>& Buffer() const { return _buffer; }
    
    using Stream::Read;
    using Stream::Write;
    
private:
    /// Dynamic data buffer.
    TVector<unsigned char> _buffer;
};

}
