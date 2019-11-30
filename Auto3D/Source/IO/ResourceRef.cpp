#include "Core/Object.h"
#include "ResourceRef.h"
#include "Stream.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

bool FResourceRef::FromString(const FString& str)
{
    return FromString(str.CString());
}

bool FResourceRef::FromString(const char* str)
{
    TVector<FString> values = FString::Split(str, ';');
    if (values.Size() == 2)
    {
        _type = values[0];
        _name = values[1];
        return true;
    }
    else
        return false;
}

void FResourceRef::FromBinary(FStream& source)
{
    _type = source.Read<FStringHash>();
    _name = source.Read<FString>();
}

FString FResourceRef::ToString() const
{
    return AObject::TypeNameFromType(_type) + ";" + _name;
}

void FResourceRef::ToBinary(FStream& dest) const
{
    dest.Write(_type);
    dest.Write(_name);
}

bool FResourceRefList::FromString(const FString& str)
{
    return FromString(str.CString());
}

bool FResourceRefList::FromString(const char* str)
{
    TVector<FString> values = FString::Split(str, ';');
    if (values.Size() >= 1)
    {
        _type = values[0];
        _names.Clear();
        for (size_t i = 1; i < values.Size(); ++i)
            _names.Push(values[i]);
        return true;
    }
    else
        return false;
}

void FResourceRefList::FromBinary(FStream& source)
{
    _type = source.Read<FStringHash>();
    size_t num = source.ReadVLE();
    _names.Clear();
    for (size_t i = 0; i < num && !source.IsEof(); ++i)
        _names.Push(source.Read<FString>());
}

FString FResourceRefList::ToString() const
{
    FString ret(AObject::TypeNameFromType(_type));
    for (auto it = _names.Begin(); it != _names.End(); ++it)
    {
        ret += ";";
        ret += *it;
    }
    return ret;
}

void FResourceRefList::ToBinary(FStream& dest) const
{
    dest.Write(_type);
    dest.WriteVLE(_names.Size());
    for (auto it = _names.Begin(); it != _names.End(); ++it)
        dest.Write(*it);
}

}
