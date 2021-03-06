//#include "Core/Object/Object.h"
#include "ResourceRef.h"
#include "IO/Stream.h"

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
        _pathName = values[1];
        return true;
    }
    else
        return false;
}

void FResourceRef::FromBinary(FStream& source)
{
    _type = source.Read<FString>();
    _pathName = source.Read<FString>();
}

FString FResourceRef::ToString() const
{
    return _type + ";" + _pathName;
}

void FResourceRef::ToBinary(FStream& dest) const
{
    dest.Write(_type);
    dest.Write(_pathName);
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
  /*  FString ret(AObject::TypeNameFromType(_type));
    for (auto it = _names.Begin(); it != _names.End(); ++it)
    {
        ret += ";";
        ret += *it;
    }
    return ret;*/
	return FString::EMPTY;
}

void FResourceRefList::ToBinary(FStream& dest) const
{
    dest.Write(_type);
    dest.WriteVLE(_names.Size());
    for (auto it = _names.Begin(); it != _names.End(); ++it)
        dest.Write(*it);
}

}
