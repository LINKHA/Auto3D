#pragma once

#include "Container/StringHash.h"
#include "Container/Vector.h"

namespace Auto3D
{

class FStream;

/// Typed resource reference for serialization.
struct AUTO_API FResourceRef
{
    /// AResource type.
    FStringHash _type;
    /// AResource name.
    FString _name;

    /// Construct.
    FResourceRef()
    {
    }

    // Copy-construct.
    FResourceRef(const FResourceRef& ref) :
        _type(ref._type),
        _name(ref._name)
    {
    }

    /// Construct from a string.
    FResourceRef(const FString& str)
    {
        FromString(str);
    }
    
    /// Construct from a C string.
    FResourceRef(const char* str)
    {
        FromString(str);
    }
    
    /// Construct with type and resource name.
    FResourceRef(FStringHash type, const FString& name_ = FString::EMPTY) :
        _type(type),
        _name(name_)
    {
    }

    /// Set from a string that contains the type and name separated by a semicolon. Return true on success.
    bool FromString(const FString& str);
    /// Set from a C string that contains the type and name separated by a semicolon. Return true on success.
    bool FromString(const char* str);
    /// Deserialize from a binary stream.
    void FromBinary(FStream& source);
    
    /// Return as a string.
    FString ToString() const;
    /// Serialize to a binary stream.
    void ToBinary(FStream& dest) const;

    /// Test for equality with another reference.
    bool operator == (const FResourceRef& rhs) const { return _type == rhs._type && _name == rhs._name; }
    /// Test for inequality with another reference.
    bool operator != (const FResourceRef& rhs) const { return !(*this == rhs); }
};

/// %TList of typed resource references for serialization.
struct AUTO_API FResourceRefList
{
    /// AResource type.
    FStringHash _type;
    /// TList of resource names.
    TVector<FString> _names;

    /// Construct.
    FResourceRefList()
    {
    }

    // Copy-construct.
    FResourceRefList(const FResourceRefList& refList) :
        _type(refList._type),
        _names(refList._names)
    {
    }

    /// Construct from a string.
    FResourceRefList(const FString& str)
    {
        FromString(str);
    }
    
    /// Construct from a C string.
    FResourceRefList(const char* str)
    {
        FromString(str);
    }

    /// Construct with type and name list.
    FResourceRefList(FStringHash type, const TVector<FString>& names = TVector<FString>()) :
        _type(type),
        _names(names)
    {
    }

    /// Set from a string that contains the type and names separated by semicolons. Return true on success.
    bool FromString(const FString& str);
    /// Set from a C string that contains the type and names separated by semicolons. Return true on success.
    bool FromString(const char* str);
    /// Deserialize from a binary stream.
    void FromBinary(FStream& source);

    /// Return as a string.
    FString ToString() const;
    /// Deserialize from a binary stream.
    void ToBinary(FStream& dest) const;

    /// Test for equality with another reference list.
    bool operator == (const FResourceRefList& rhs) const { return _type == rhs._type && _names == rhs._names; }
    /// Test for inequality with another reference list.
    bool operator != (const FResourceRefList& rhs) const { return !(*this == rhs); }
};

}
