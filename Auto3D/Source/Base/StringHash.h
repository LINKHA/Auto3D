#pragma once

#include "../Base/String.h"

namespace Auto3D
{

/// 32-bit case-insensitive hash value for a string.
class AUTO_API StringHash
{
public:
    /// Construct with zero value.
    StringHash() :
        _value(0)
    {
    }
    
    /// Copy-construct.
    StringHash(const StringHash& hash) :
        _value(hash._value)
    {
    }
    
    /// Construct with an initial value.
    explicit StringHash(unsigned value_) :
        _value(value_)
    {
    }
    
    /// Construct from a string case-insensitively.
    explicit StringHash(const String& str) :
        _value(String::CaseInsensitiveHash(str.CString()))
    {
    }
    
    /// Construct from a C string case-insensitively.
    explicit StringHash(const char* str) :
        _value(String::CaseInsensitiveHash(str))
    {
    }
    
    /// Construct from a C string case-insensitively.
    explicit StringHash(char* str) :
        _value(String::CaseInsensitiveHash(str))
    {
    }
    
    /// Assign from another hash.
    StringHash& operator = (const StringHash& rhs)
    {
        _value = rhs._value;
        return *this;
    }
    
    /// Assign from a string.
    StringHash& operator = (const String& rhs)
    {
        _value = String::CaseInsensitiveHash(rhs.CString());
        return *this;
    }
    
    /// Assign from a C string.
    StringHash& operator = (const char* rhs)
    {
        _value = String::CaseInsensitiveHash(rhs);
        return *this;
    }
    
    /// Assign from a C string.
    StringHash& operator = (char* rhs)
    {
        _value = String::CaseInsensitiveHash(rhs);
        return *this;
    }
    
    /// Add a hash.
    StringHash operator + (const StringHash& rhs) const
    {
        StringHash ret;
        ret._value = _value + rhs._value;
        return ret;
    }
    
    /// Add-assign a hash.
    StringHash& operator += (const StringHash& rhs)
    {
        _value += rhs._value;
        return *this;
    }
    
    // Test for equality with another hash.
    bool operator == (const StringHash& rhs) const { return _value == rhs._value; }
    /// Test for inequality with another hash.
    bool operator != (const StringHash& rhs) const { return _value != rhs._value; }
    /// Test if less than another hash.
    bool operator < (const StringHash& rhs) const { return _value < rhs._value; }
    /// Test if greater than another hash.
    bool operator > (const StringHash& rhs) const { return _value > rhs._value; }
    /// Return true if nonzero hash value.
    operator bool () const { return _value != 0; }
    /// Return hash value.
    unsigned Value() const { return _value; }
    /// Return as string.
    String ToString() const;
    /// Return hash value for HashSet & HashMap.
    unsigned ToHash() const { return _value; }
    
    /// Calculate hash value case-insensitively from a C string.
    static unsigned Calculate(const char* str);
    
    /// Zero hash.
    static const StringHash ZERO;
    
private:
    /// Hash value.
    unsigned _value;
};

}
