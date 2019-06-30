#pragma once

#include "../AutoConfig.h"
#include <cassert>
#include <cstddef>

namespace Auto3D
{

class String;

/// Wide character string. Only meant for converting from String and passing to the operating system where necessary.
class AUTO_API WString
{
public:
    /// Construct empty.
    WString();
    /// Construct from a string.
    WString(const String& str);
    /// Destruct.
    ~WString();
    
    /// Return char at index.
    wchar_t& operator [] (size_t index) { assert(index < _length); return _buffer[index]; }
    /// Return const char at index.
    const wchar_t& operator [] (size_t index) const { assert(index < _length); return _buffer[index]; }
    /// Return char at index.
    wchar_t& At(size_t index) { assert(index < _length); return _buffer[index]; }
    /// Return const char at index.
    const wchar_t& At(size_t index) const { assert(index < _length); return _buffer[index]; }
    /// Resize the string.
    void Resize(size_t newLength);
    /// Return whether the string is zero characters long.
    bool IsEmpty() const { return _length == 0; }
    /// Return number of characters.
    size_t Length() const { return _length; }
    /// Return character data.
    const wchar_t* CString() const { return _buffer; }
    
private:
    /// String length.
    size_t _length;
    /// String buffer, null if not allocated.
    wchar_t* _buffer;
};

}
