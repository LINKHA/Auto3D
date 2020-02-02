#include "Math/MathDef.h"
#include "Stream.h"
#include "JSONValue.h"
#include "ObjectRef.h"
#include "ResourceRef.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

FStream::FStream() :
    _position(0),
    _size(0)
{
}

FStream::FStream(size_t numBytes) :
    _position(0),
    _size(numBytes)
{
}

FStream::~FStream()
{
}

void FStream::SetName(const FString& newName)
{
    _pathName = newName;
}

void FStream::SetName(const char* newName)
{
    _pathName = newName;
}

unsigned FStream::ReadVLE()
{
    unsigned ret;
    unsigned char byte;
    
    byte = Read<unsigned char>();
    ret = byte & 0x7f;
    if (byte < 0x80)
        return ret;
    
    byte = Read<unsigned char>();
    ret |= ((unsigned)(byte & 0x7f)) << 7;
    if (byte < 0x80)
        return ret;
    
    byte = Read<unsigned char>();
    ret |= ((unsigned)(byte & 0x7f)) << 14;
    if (byte < 0x80)
        return ret;
    
    byte = Read<unsigned char>();
    ret |= ((unsigned)byte) << 21;
    return ret;
}

FString FStream::ReadLine()
{
    FString ret;
    
    while (!IsEof())
    {
        char c = Read<char>();
        if (c == 10)
            break;
        if (c == 13)
        {
            // Peek next char to see if it's 10, and skip it too
            if (!IsEof())
            {
                char next = Read<char>();
                if (next != 10)
                    Seek(_position - 1);
            }
            break;
        }
        
        ret += c;
    }
    
    return ret;
}

FString FStream::ReadFileID()
{
    FString ret;
    ret.Resize(4);
    Read(&ret[0], 4);
    return ret;
}

TVector<unsigned char> FStream::ReadBuffer()
{
    TVector<unsigned char> ret(ReadVLE());
    if (ret.Size())
        Read(&ret[0], ret.Size());
    return ret;
}

template<> bool FStream::Read<bool>()
{
    return Read<unsigned char>() != 0;
}

template<> FString FStream::Read<FString>()
{
    FString ret;
    
    while (!IsEof())
    {
        char c = Read<char>();
        if (!c)
            break;
        else
            ret += c;
    }
    
    return ret;
}

template<> FStringHash FStream::Read<FStringHash>()
{
    return FStringHash(Read<unsigned>());
}

template<> FResourceRef FStream::Read<FResourceRef>()
{
    FResourceRef ret;
    ret.FromBinary(*this);
    return ret;
}

template<> FResourceRefList FStream::Read<FResourceRefList>()
{
    FResourceRefList ret;
    ret.FromBinary(*this);
    return ret;
}

template<> FObjectRef FStream::Read<FObjectRef>()
{
    FObjectRef ret;
    ret._id = Read<unsigned>();
    return ret;
}

template<> FJSONValue FStream::Read<FJSONValue>()
{
    FJSONValue ret;
    ret.FromBinary(*this);
    return ret;
}

void FStream::WriteFileID(const FString& value)
{
    Write(value.CString(), Min((int)value.Length(), 4));
    for (size_t i = value.Length(); i < 4; ++i)
        Write(' ');
}

void FStream::WriteBuffer(const TVector<unsigned char>& value)
{
    size_t numBytes = value.Size();
    
    WriteVLE(numBytes);
    if (numBytes)
        Write(&value[0], numBytes);
}

void FStream::WriteVLE(size_t value)
{
    unsigned char _data[4];
    
    if (value < 0x80)
        Write((unsigned char)value);
    else if (value < 0x4000)
    {
        _data[0] = (unsigned char)value | 0x80;
        _data[1] = (unsigned char)(value >> 7);
        Write(_data, 2);
    }
    else if (value < 0x200000)
    {
        _data[0] = (unsigned char)value | 0x80;
        _data[1] = (unsigned char)((value >> 7) | 0x80);
        _data[2] = (unsigned char)(value >> 14);
        Write(_data, 3);
    }
    else
    {
        _data[0] = (unsigned char)value | 0x80;
        _data[1] = (unsigned char)((value >> 7) | 0x80);
        _data[2] = (unsigned char)((value >> 14) | 0x80);
        _data[3] = (unsigned char)(value >> 21);
        Write(_data, 4);
    }
}

void FStream::WriteLine(const FString& value)
{
    Write(value.CString(), value.Length());
    Write('\r');
    Write('\n');
}

template<> void FStream::Write<bool>(const bool& value)
{
    Write<unsigned char>(value ? 1 : 0);
}

template<> void FStream::Write<FString>(const FString& value)
{
    // Write content and null terminator
    Write(value.CString(), value.Length() + 1);
}

template<> void FStream::Write<FStringHash>(const FStringHash& value)
{
    Write(value.Value());
}

template<> void FStream::Write<FResourceRef>(const FResourceRef& value)
{
    value.ToBinary(*this);
}

template<> void FStream::Write<FResourceRefList>(const FResourceRefList& value)
{
    value.ToBinary(*this);
}

template<> void FStream::Write<FObjectRef>(const FObjectRef& value)
{
    Write(value._id);
}

template<> void FStream::Write<FJSONValue>(const FJSONValue& value)
{
    value.ToBinary(*this);
}

}
