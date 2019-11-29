#pragma once

#include "Container/String.h"

namespace Auto3D
{

/// 32-bit case-insensitive hash value for a string.
class AUTO_API FStringHash
{
public:
    /// Construct with zero value.
    FStringHash() :
        _value(0)
    {
    }
    
    /// Copy-construct.
    FStringHash(const FStringHash& hash) :
        _value(hash._value)
    {
    }
    
    /// Construct with an initial value.
    explicit FStringHash(unsigned value) :
        _value(value)
    {
    }
    
    /// Construct from a string case-insensitively.
    explicit FStringHash(const FString& str) :
        _value(FString::CaseInsensitiveHash(str.CString()))
    {
    }
    
    /// Construct from a C string case-insensitively.
    explicit FStringHash(const char* str) :
        _value(FString::CaseInsensitiveHash(str))
    {
    }
    
    /// Construct from a C string case-insensitively.
    explicit FStringHash(char* str) :
        _value(FString::CaseInsensitiveHash(str))
    {
    }
    
    /// Assign from another hash.
    FStringHash& operator = (const FStringHash& rhs)
    {
        _value = rhs._value;
        return *this;
    }
    
    /// Assign from a string.
    FStringHash& operator = (const FString& rhs)
    {
        _value = FString::CaseInsensitiveHash(rhs.CString());
        return *this;
    }
    
    /// Assign from a C string.
    FStringHash& operator = (const char* rhs)
    {
        _value = FString::CaseInsensitiveHash(rhs);
        return *this;
    }
    
    /// Assign from a C string.
    FStringHash& operator = (char* rhs)
    {
        _value = FString::CaseInsensitiveHash(rhs);
        return *this;
    }
    
    /// Add a hash.
    FStringHash operator + (const FStringHash& rhs) const
    {
        FStringHash ret;
        ret._value = _value + rhs._value;
        return ret;
    }
    
    /// Add-assign a hash.
    FStringHash& operator += (const FStringHash& rhs)
    {
        _value += rhs._value;
        return *this;
    }
    
    // Test for equality with another hash.
    bool operator == (const FStringHash& rhs) const { return _value == rhs._value; }
    /// Test for inequality with another hash.
    bool operator != (const FStringHash& rhs) const { return _value != rhs._value; }
    /// Test if less than another hash.
    bool operator < (const FStringHash& rhs) const { return _value < rhs._value; }
    /// Test if greater than another hash.
    bool operator > (const FStringHash& rhs) const { return _value > rhs._value; }
    /// Return true if nonzero hash value.
    operator bool () const { return _value != 0; }
    /// Return hash value.
    unsigned Value() const { return _value; }
    /// Return as string.
    FString ToString() const;
    /// Return hash value for THashSet & THashMap.
    unsigned ToHash() const { return _value; }
    
    /// Calculate hash value case-insensitively from a C string.
    static unsigned Calculate(const char* str, unsigned hash = 0);
    
    /// Zero hash.
    static const FStringHash ZERO;
    
private:
    /// Hash value.
    unsigned _value;
};

}
