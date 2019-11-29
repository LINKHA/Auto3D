#include "Core/Object.h"
#include "ResourceRef.h"
#include "Stream.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

bool ResourceRef::FromString(const String& str)
{
    return FromString(str.CString());
}

bool ResourceRef::FromString(const char* str)
{
    Vector<String> values = String::Split(str, ';');
    if (values.Size() == 2)
    {
        _type = values[0];
        _name = values[1];
        return true;
    }
    else
        return false;
}

void ResourceRef::FromBinary(Stream& source)
{
    _type = source.Read<StringHash>();
    _name = source.Read<String>();
}

String ResourceRef::ToString() const
{
    return AObject::TypeNameFromType(_type) + ";" + _name;
}

void ResourceRef::ToBinary(Stream& dest) const
{
    dest.Write(_type);
    dest.Write(_name);
}

bool ResourceRefList::FromString(const String& str)
{
    return FromString(str.CString());
}

bool ResourceRefList::FromString(const char* str)
{
    Vector<String> values = String::Split(str, ';');
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

void ResourceRefList::FromBinary(Stream& source)
{
    _type = source.Read<StringHash>();
    size_t num = source.ReadVLE();
    _names.Clear();
    for (size_t i = 0; i < num && !source.IsEof(); ++i)
        _names.Push(source.Read<String>());
}

String ResourceRefList::ToString() const
{
    String ret(AObject::TypeNameFromType(_type));
    for (auto it = _names.Begin(); it != _names.End(); ++it)
    {
        ret += ";";
        ret += *it;
    }
    return ret;
}

void ResourceRefList::ToBinary(Stream& dest) const
{
    dest.Write(_type);
    dest.WriteVLE(_names.Size());
    for (auto it = _names.Begin(); it != _names.End(); ++it)
        dest.Write(*it);
}

}
