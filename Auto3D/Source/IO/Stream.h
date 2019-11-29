#pragma once

#include "Container/String.h"
#include "Container/Ptr.h"

namespace Auto3D
{

class JSONValue;
class FStringHash;
template <typename _Ty> class TVector;
struct ObjectRef;
struct ResourceRef;
struct ResourceRefList;

/// Abstract stream for reading and writing.
class AUTO_API Stream : public FRefCounted
{
public:
    /// Default-construct with zero _size.
    Stream();
    /// Construct with defined byte _size.
    Stream(size_t numBytes);
    /// Destruct.
    virtual ~Stream();
    
    /// Read bytes from the stream. Return number of bytes actually read.
    virtual size_t Read(void* dest, size_t numBytes) = 0;
    /// Set _position in bytes from the beginning of the stream. Return the _position after the seek.
    virtual size_t Seek(size_t position) = 0;
    /// Write bytes to the stream. Return number of bytes actually written.
    virtual size_t Write(const void* _data, size_t _size) = 0;
    /// Return whether read operations are allowed.
    virtual bool IsReadable() const = 0;
    /// Return whether write operations are allowed.
    virtual bool IsWritable() const = 0;

    /// Change the stream name.
    void SetName(const FString& newName);
    /// Change the stream name.
    void SetName(const char* newName);
    /// Read a variable-length encoded unsigned integer, which can use 29 bits maximum.
    unsigned ReadVLE();
    /// Read a text line.
    FString ReadLine();
    /// Read a 4-character file ID.
    FString ReadFileID();
    /// Read a byte buffer, with _size prepended as a VLE value.
    TVector<unsigned char> ReadBuffer();
    /// Write a four-letter file ID. If the string is not long enough, spaces will be appended.
    void WriteFileID(const FString& value);
    /// Write a byte buffer, with _size encoded as VLE.
    void WriteBuffer(const TVector<unsigned char>& buffer);
    /// Write a variable-length encoded unsigned integer, which can use 29 bits maximum.
    void WriteVLE(size_t value);
    /// Write a text line. Char codes 13 & 10 will be automatically appended.
    void WriteLine(const FString& value);

    /// Write a value, template version.
    template <typename _Ty> void Write(const _Ty& value) { Write(&value, sizeof value); }

    /// Read a value, template version.
    template <typename _Ty> _Ty Read()
    {
        _Ty ret;
        Read(&ret, sizeof ret);
        return ret;
    }
    
    /// Return the stream name.
    const FString& FName() const { return _name; }
    /// Return current _position in bytes.
    size_t Position() const { return _position; }
    /// Return _size in bytes.
    size_t Size() const { return _size; }
    /// Return whether the end of stream has been reached.
    bool IsEof() const { return _position >= _size; }
    
protected:
    /// Stream _position.
    size_t _position;
    /// Stream _size.
    size_t _size;
    /// Stream name.
    FString _name;
};

template<> AUTO_API bool Stream::Read();
template<> AUTO_API FString Stream::Read();
template<> AUTO_API FStringHash Stream::Read();
template<> AUTO_API ResourceRef Stream::Read();
template<> AUTO_API ResourceRefList Stream::Read();
template<> AUTO_API ObjectRef Stream::Read();
template<> AUTO_API JSONValue Stream::Read();
template<> AUTO_API void Stream::Write(const bool& value);
template<> AUTO_API void Stream::Write(const FString& value);
template<> AUTO_API void Stream::Write(const FStringHash& value);
template<> AUTO_API void Stream::Write(const ResourceRef& value);
template<> AUTO_API void Stream::Write(const ResourceRefList& value);
template<> AUTO_API void Stream::Write(const ObjectRef& value);
template<> AUTO_API void Stream::Write(const JSONValue& value);

}
