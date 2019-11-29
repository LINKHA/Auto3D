#include "String.h"
#include "Swap.h"
#include "Vector.h"
#include "WString.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Debug/DebugNew.h"

namespace Auto3D
{

char FString::endZero = 0;

const FString FString::EMPTY;

FString::FString(const char* str, size_t numChars) :
    _buffer(nullptr)
{
    Resize(numChars);
    CopyChars(Buffer(), str, numChars);
}

FString::FString(const wchar_t* str) :
    _buffer(nullptr)
{
    SetUTF8FromWChar(str);
}

FString::FString(wchar_t* str) :
    _buffer(nullptr)
{
    SetUTF8FromWChar(str);
}

FString::FString(const FWString& str) :
    _buffer(nullptr)
{
    SetUTF8FromWChar(str.CString());
}

FString::FString(int value) :
    _buffer(nullptr)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%d", value);
    *this = tempBuffer;
}

FString::FString(short value) :
    _buffer(nullptr)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%d", value);
    *this = tempBuffer;
}

FString::FString(long value) :
    _buffer(nullptr)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%ld", value);
    *this = tempBuffer;
}
    
FString::FString(long long value) :
    _buffer(nullptr)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%lld", value);
    *this = tempBuffer;
}

FString::FString(unsigned value) :
    _buffer(nullptr)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%u", value);
    *this = tempBuffer;
}

FString::FString(unsigned short value) :
    _buffer(nullptr)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%u", value);
    *this = tempBuffer;
}

FString::FString(unsigned long value) :
    _buffer(nullptr)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%lu", value);
    *this = tempBuffer;
}
    
FString::FString(unsigned long long value) :
    _buffer(nullptr)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%llu", value);
    *this = tempBuffer;
}

FString::FString(float value) :
    _buffer(nullptr)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%g", value);
    *this = tempBuffer;
}

FString::FString(double value) :
    _buffer(nullptr)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%.15g", value);
    *this = tempBuffer;
}

FString::FString(bool value) :
    _buffer(nullptr)
{
    if (value)
        *this = "true";
    else
        *this = "false";
}

FString::FString(char value) :
    _buffer(nullptr)
{
    Resize(1);
    Buffer()[0] = value;
}

FString::FString(char value, size_t numChars) :
    _buffer(nullptr)
{
    Resize(numChars);
    for (Iterator it = Begin(); it != End(); ++it)
        *it = value;
}

FString::~FString()
{
    delete[] _buffer;
}

FString& FString::operator = (const FString& rhs)
{
    Resize(rhs.Length());
    CopyChars(Buffer(), rhs.Buffer(), rhs.Length());
    
    return *this;
}

FString& FString::operator = (const char* rhs)
{
    size_t rhsLength = CStringLength(rhs);
    Resize(rhsLength);
    CopyChars(Buffer(), rhs, rhsLength);
    
    return *this;
}

FString& FString::operator = (char* rhs)
{
    size_t rhsLength = CStringLength(rhs);
    Resize(rhsLength);
    CopyChars(Buffer(), rhs, rhsLength);
    
    return *this;
}

FString& FString::operator += (const FString& rhs)
{
    size_t oldLength = Length();
    Resize(Length() + rhs.Length());
    CopyChars(Buffer() + oldLength, rhs.Buffer(), rhs.Length());
    
    return *this;
}

FString& FString::operator += (const char* rhs)
{
    size_t rhsLength = CStringLength(rhs);
    size_t oldLength = Length();
    Resize(Length() + rhsLength);
    CopyChars(Buffer() + oldLength, rhs, rhsLength);
    
    return *this;
}

FString& FString::operator += (char* rhs)
{
    size_t rhsLength = CStringLength(rhs);
    size_t oldLength = Length();
    Resize(Length() + rhsLength);
    CopyChars(Buffer() + oldLength, rhs, rhsLength);
    
    return *this;
}

FString& FString::operator += (char rhs)
{
    size_t oldLength = Length();
    Resize(Length() + 1);
    Buffer()[oldLength] = rhs;
    
    return *this;
}

FString& FString::operator += (int rhs)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%d", rhs);
    *this += tempBuffer;
    
    return *this;
}

FString& FString::operator += (short rhs)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%d", rhs);
    *this += tempBuffer;
    
    return *this;
}

FString& FString::operator += (long rhs)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%ld", rhs);
    *this += tempBuffer;
    
    return *this;
}

FString& FString::operator += (long long rhs)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%lld", rhs);
    *this += tempBuffer;
    
    return *this;
}

FString& FString::operator += (unsigned rhs)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%u", rhs);
    *this += tempBuffer;
    
    return *this;
}

FString& FString::operator += (unsigned short rhs)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%u", rhs);
    *this += tempBuffer;
    
    return *this;
}

FString& FString::operator += (unsigned long rhs)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%lu", rhs);
    *this += tempBuffer;
    
    return *this;
}

FString& FString::operator += (unsigned long long rhs)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%llu", rhs);
    *this += tempBuffer;
    
    return *this;
}

FString& FString::operator += (float rhs)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%g", rhs);
    *this += tempBuffer;
    
    return *this;
}

FString& FString::operator += (double rhs)
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%g", rhs);
    *this += tempBuffer;
    
    return *this;
}

FString& FString::operator += (bool rhs)
{
    if (rhs)
        *this += "true";
    else
         *this += "false";
    
    return *this;
}

FString FString::operator + (const FString& rhs) const
{
    FString ret;
    ret.Resize(Length() + rhs.Length());
    CopyChars(ret.Buffer(), Buffer(), Length());
    CopyChars(ret.Buffer() + Length(), rhs.Buffer(), rhs.Length());
    
    return ret;
}

FString FString::operator + (const char* rhs) const
{
    size_t rhsLength = CStringLength(rhs);
    FString ret;
    ret.Resize(Length() + rhsLength);
    CopyChars(ret.Buffer(), Buffer(), Length());
    CopyChars(ret.Buffer() + Length(), rhs, rhsLength);
    
    return ret;
}

FString FString::operator + (char rhs) const
{
    FString ret(*this);
    ret += rhs;
    
    return ret;
}

void FString::Replace(char replaceThis, char replaceWith, bool caseSensitive)
{
    if (caseSensitive)
    {
        for (Iterator it = Begin(); it != End(); ++it)
        {
            if (*it == replaceThis)
                *it = replaceWith;
        }
    }
    else
    {
        replaceThis = Auto3D::ToLower(replaceThis);
        for (Iterator it = Begin(); it != End(); ++it)
        {
            if (Auto3D::ToLower(*it) == replaceThis)
                *it = replaceWith;
        }
    }
}

void FString::Replace(const FString& replaceThis, const FString& replaceWith, bool caseSensitive)
{
    size_t nextPos = 0;
    
    while (nextPos < Length())
    {
        size_t pos = Find(replaceThis, nextPos, caseSensitive);
        if (pos == NPOS)
            break;
        Replace(pos, replaceThis.Length(), replaceWith);
        nextPos = pos + replaceWith.Length();
    }
}

void FString::Replace(size_t pos, size_t numChars, const FString& replaceWith)
{
    // If substring is illegal, do nothing
    if (pos + numChars > Length())
        return;
    
    Replace(pos, numChars, replaceWith.Buffer(), replaceWith.Length());
}

void FString::Replace(size_t pos, size_t numChars, const char* replaceWith)
{
    // If substring is illegal, do nothing
    if (pos + numChars > Length())
        return;
    
    Replace(pos, numChars, replaceWith, CStringLength(replaceWith));
}

FString::Iterator FString::Replace(const FString::Iterator& start, const FString::Iterator& end, const FString& replaceWith)
{
    size_t pos = start - Begin();
    if (pos >= Length())
        return End();
    size_t numChars = end - start;
    Replace(pos, numChars, replaceWith);
    
    return Begin() + pos;
}

FString FString::Replaced(char replaceThis, char replaceWith, bool caseSensitive) const
{
    FString ret(*this);
    ret.Replace(replaceThis, replaceWith, caseSensitive);
    return ret;
}

FString FString::Replaced(const FString& replaceThis, const FString& replaceWith,  bool caseSensitive) const
{
    FString ret(*this);
    ret.Replace(replaceThis, replaceWith, caseSensitive);
    return ret;
}

FString& FString::Append(const FString& str)
{
    return *this += str;
}

FString& FString::Append(const char* str)
{
    return *this += str;
}

FString& FString::Append(char c)
{
    return *this += c;
}

FString& FString::Append(const char* str, size_t numChars)
{
    if (str)
    {
        size_t oldLength = Length();
        Resize(oldLength + numChars);
        CopyChars(&Buffer()[oldLength], str, numChars);
    }
    return *this;
}

void FString::Insert(size_t pos, const FString& str)
{
    if (pos > Length())
        pos = Length();
    
    if (pos == Length())
        (*this) += str;
    else
        Replace(pos, 0, str);
}

void FString::Insert(size_t pos, char c)
{
    if (pos > Length())
        pos = Length();
    
    if (pos == Length())
        (*this) += c;
    else
    {
        size_t oldLength = Length();
        Resize(Length() + 1);
        MoveRange(pos + 1, pos, oldLength - pos);
        Buffer()[pos] = c;
    }
}

FString::Iterator FString::Insert(const FString::Iterator& dest, const FString& str)
{
    size_t pos = dest - Begin();
    if (pos > Length())
        pos = Length();
    Insert(pos, str);
    
    return Begin() + pos;
}

FString::Iterator FString::Insert(const FString::Iterator& dest, const FString::Iterator& start, const FString::Iterator& end)
{
    size_t pos = dest - Begin();
    if (pos > Length())
        pos = Length();
    size_t numChars = end - start;
    Replace(pos, 0, &(*start), numChars);
    
    return Begin() + pos;
}

FString::Iterator FString::Insert(const FString::Iterator& dest, char c)
{
    size_t pos = dest - Begin();
    if (pos > Length())
        pos = Length();
    Insert(pos, c);
    
    return Begin() + pos;
}

void FString::Erase(size_t pos, size_t numChars)
{
    Replace(pos, numChars, FString::EMPTY);
}

FString::Iterator FString::Erase(const FString::Iterator& it)
{
    size_t pos = it - Begin();
    if (pos >= Length())
        return End();
    Erase(pos);
    
    return Begin() + pos;
}

FString::Iterator FString::Erase(const FString::Iterator& start, const FString::Iterator& end)
{
    size_t pos = start - Begin();
    if (pos >= Length())
        return End();
    size_t numChars = end - start;
    Erase(pos, numChars);
    
    return Begin() + pos;
}

void FString::Resize(size_t newLength)
{
    if (!_buffer)
    {
        // If zero length requested, do not allocate buffer yet
        if (!newLength)
            return;
        
        // Calculate initial capacity
        size_t capacity = newLength + 1;
        if (capacity < MIN_CAPACITY)
            capacity = MIN_CAPACITY;
        
        _buffer = new char[capacity + 2 * sizeof(size_t)];
        SetCapacity(capacity);
    }
    else
    {
        size_t capacity = Capacity();

        if (newLength && capacity < newLength + 1)
        {
            // Increase the capacity with half each time it is exceeded
            while (capacity < newLength + 1)
                capacity += (capacity + 1) >> 1;
            
            char* newBuffer = new char[capacity + 2 * sizeof(size_t)];
            // Move the existing data to the new buffer, then delete the old buffer
            if (Length())
                CopyChars(newBuffer + 2 * sizeof(size_t), Buffer(), Length());
            delete[] _buffer;
            
            _buffer = newBuffer;
            SetCapacity(capacity);
        }
    }

    SetLength(newLength);
    Buffer()[newLength] = 0;
}

void FString::Reserve(size_t newCapacity)
{
    size_t length = Length();
    if (newCapacity < length + 1)
        newCapacity = length + 1;
    if (newCapacity == Capacity())
        return;
    
    char* newBuffer = new char[newCapacity + 2 * sizeof(size_t)];
    // Move the existing data to the new buffer (including the end zero), then delete the old buffer
    if (length)
        CopyChars(newBuffer + 2 * sizeof(size_t), Buffer(), length + 1);
    delete[] _buffer;

    _buffer = newBuffer;
    SetLength(length);
    SetCapacity(newCapacity);
}

void FString::Compact()
{
    if (Capacity())
        Reserve(Length() + 1);
}

void FString::Clear()
{
    Resize(0);
}

void FString::Swap(FString& str)
{
    Auto3D::Swap(_buffer, str._buffer);
}

FString& FString::AppendWithFormat(const char* formatStr, ... )
{
    va_list args;
    va_start(args, formatStr);
    AppendWithFormatArgs(formatStr, args);
    va_end(args);
    return *this;
}

FString& FString::AppendWithFormatArgs(const char* formatStr, va_list args)
{
    size_t pos = 0, lastPos = 0;
    size_t length = CStringLength(formatStr);

    for (;;)
    {
        // Scan the format string and find %a argument where a is one of d, f, s ...
        while (pos < length && formatStr[pos] != '%')
            pos++;
        Append(formatStr + lastPos, pos - lastPos);
        if (pos >= length)
            break;
        
        char argType = formatStr[pos + 1];
        pos += 2;
        lastPos = pos;
        
        switch (argType)
        {
        // Integer
        case 'd':
        case 'i':
            {
                int arg = va_arg(args, int);
                Append(FString(arg));
                break;
            }
            
        // Unsigned
        case 'u':
            {
                unsigned arg = va_arg(args, unsigned);
                Append(FString(arg));
                break;
            }
            
        // Real
        case 'f':
            {
                double arg = va_arg(args, double);
                Append(FString(arg));
                break;
            }
            
        // Character
        case 'c':
            {
                int arg = va_arg(args, int);
                Append((char)arg);
                break;
            }
            
        // C string
        case 's':
            {
                char* arg = va_arg(args, char*);
                Append(arg);
                break;
            }
            
        // Hex
        case 'x':
            {
                char buf[CONVERSION_BUFFER_LENGTH];
                int arg = va_arg(args, int);
                int arglen = sprintf(buf, "%x", arg);
                Append(buf, arglen);
                break;
            }
            
        // Pointer
        case 'p':
            {
                char buf[CONVERSION_BUFFER_LENGTH];
                int arg = va_arg(args, int);
                int arglen = sprintf(buf, "%p", reinterpret_cast<void*>(arg));
                Append(buf, arglen);
                break;
            }
            
        case '%':
            {
                Append("%", 1);
                break;
            }
        }
    }

    return *this;
}

FString FString::Substring(size_t pos) const
{
    if (pos < Length())
    {
        FString ret;
        ret.Resize(Length() - pos);
        CopyChars(ret.Buffer(), Buffer() + pos, ret.Length());
        
        return ret;
    }
    else
        return FString();
}

FString FString::Substring(size_t pos, size_t numChars) const
{
    if (pos < Length())
    {
        FString ret;
        if (pos + numChars > Length())
            numChars = Length() - pos;
        ret.Resize(numChars);
        CopyChars(ret.Buffer(), Buffer() + pos, ret.Length());
        
        return ret;
    }
    else
        return FString();
}

FString FString::Trimmed() const
{
    size_t trimStart = 0;
    size_t trimEnd = Length();
    
    while (trimStart < trimEnd)
    {
        char c = Buffer()[trimStart];
        if (c != ' ' && c != 9)
            break;
        ++trimStart;
    }
    while (trimEnd > trimStart)
    {
        char c = Buffer()[trimEnd - 1];
        if (c != ' ' && c != 9)
            break;
        --trimEnd;
    }
    
    return Substring(trimStart, trimEnd - trimStart);
}

FString FString::ToLower() const
{
    FString ret(*this);

    for (Iterator it = ret.Begin(); it != ret.End(); ++it)
        *it = Auto3D::ToLower(*it);
    
    return ret;
}

FString FString::ToUpper() const
{
    FString ret(*this);

    for (Iterator it = ret.Begin(); it != ret.End(); ++it)
        *it = Auto3D::ToUpper(*it);
    
    return ret;
}

TVector<FString> FString::Split(char separator) const
{
    return Split(CString(), separator);
}

size_t FString::Find(char c, size_t startPos, bool caseSensitive) const
{
    if (caseSensitive)
    {
        for (size_t i = startPos; i < Length(); ++i)
        {
            if (Buffer()[i] == c)
                return i;
        }
    }
    else
    {
        c = Auto3D::ToLower(c);
        for (size_t i = startPos; i < Length(); ++i)
        {
            if (Auto3D::ToLower(Buffer()[i]) == c)
                return i;
        }
    }
    
    return NPOS;
}

size_t FString::Find(const FString& str, size_t startPos, bool caseSensitive) const
{
    if (!str.Length() || str.Length() > Length())
        return NPOS;
    
    char first = str.Buffer()[0];
    if (!caseSensitive)
        first = Auto3D::ToLower(first);

    for (size_t i = startPos; i <= Length() - str.Length(); ++i)
    {
        char c = Buffer()[i];
        if (!caseSensitive)
            c = Auto3D::ToLower(c);

        if (c == first)
        {
            size_t skip = NPOS;
            bool found = true;
            for (size_t j = 1; j < str.Length(); ++j)
            {
                c = Buffer()[i + j];
                char d = str.Buffer()[j];
                if (!caseSensitive)
                {
                    c = Auto3D::ToLower(c);
                    d = Auto3D::ToLower(d);
                }

                if (skip == NPOS && c == first)
                    skip = i + j - 1;

                if (c != d)
                {
                    found = false;
                    if (skip != NPOS)
                        i = skip;
                    break;
                }
            }
            if (found)
                return i;
        }
    }
    
    return NPOS;
}

size_t FString::FindLast(char c, size_t startPos, bool caseSensitive) const
{
    if (startPos >= Length())
        startPos = Length() - 1;
    
    if (caseSensitive)
    {
        for (size_t i = startPos; i < Length(); --i)
        {
            if (Buffer()[i] == c)
                return i;
        }
    }
    else
    {
        c = Auto3D::ToLower(c);
        for (size_t i = startPos; i < Length(); --i)
        {
            if (Auto3D::ToLower(Buffer()[i]) == c)
                return i;
        }
    }
    
    return NPOS;
}

size_t FString::FindLast(const FString& str, size_t startPos, bool caseSensitive) const
{
    if (!str.Length() || str.Length() > Length())
        return NPOS;
    if (startPos > Length() - str.Length())
        startPos = Length() - str.Length();
    
    char first = str.Buffer()[0];
    if (!caseSensitive)
        first = Auto3D::ToLower(first);

    for (size_t i = startPos; i < Length(); --i)
    {
        char c = Buffer()[i];
        if (!caseSensitive)
            c = Auto3D::ToLower(c);

        if (c == first)
        {
            bool found = true;
            for (size_t j = 1; j < str.Length(); ++j)
            {
                c = Buffer()[i + j];
                char d = str.Buffer()[j];
                if (!caseSensitive)
                {
                    c = Auto3D::ToLower(c);
                    d = Auto3D::ToLower(d);
                }

                if (c != d)
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return i;
        }
    }
    
    return NPOS;
}

bool FString::StartsWith(const FString& str, bool caseSensitive) const
{
    return Find(str, 0, caseSensitive) == 0;
}

bool FString::EndsWith(const FString& str, bool caseSensitive) const
{
    size_t pos = FindLast(str, Length() - 1, caseSensitive);
    return pos != NPOS && pos == Length() - str.Length();
}

bool FString::ToBool() const
{
    return ToBool(CString());
}

int FString::ToInt() const
{
    return ToInt(CString());
}

unsigned FString::ToUInt() const
{
    return ToUInt(CString());
}

float FString::ToFloat() const
{
    return ToFloat(CString());
}

void FString::SetUTF8FromLatin1(const char* str)
{
    char temp[7];
    
    Clear();
    
    if (!str)
        return;
    
    while (*str)
    {
        char* dest = temp;
        EncodeUTF8(dest, *str++);
        *dest = 0;
        Append(temp);
    }
}

void FString::SetUTF8FromWChar(const wchar_t* str)
{
    char temp[7];
    
    Clear();
    
    if (!str)
        return;
    
    #ifdef _WIN32
    while (*str)
    {
        unsigned unicodeChar = DecodeUTF16(str);
        char* dest = temp;
        EncodeUTF8(dest, unicodeChar);
        *dest = 0;
        Append(temp);
    }
    #else
    while (*str)
    {
        char* dest = temp;
        EncodeUTF8(dest, *str++);
        *dest = 0;
        Append(temp);
    }
    #endif
}

size_t FString::LengthUTF8() const
{
    size_t ret = 0;
    
    const char* src = Buffer();
    if (!src)
        return ret;
    const char* end = Buffer() + Length();
    
    while (src < end)
    {
        DecodeUTF8(src);
        ++ret;
    }
    
    return ret;
}

size_t FString::ByteOffsetUTF8(size_t index) const
{
    size_t byteOffset = 0;
    size_t utfPos = 0;
    
    while (utfPos < index && byteOffset < Length())
    {
        NextUTF8Char(byteOffset);
        ++utfPos;
    }
    
    return byteOffset;
}

unsigned FString::NextUTF8Char(size_t& byteOffset) const
{
    if (!_buffer)
        return 0;
    
    const char* src = Buffer() + byteOffset;
    unsigned ret = DecodeUTF8(src);
    byteOffset = src - Buffer();
    
    return ret;
}

unsigned FString::AtUTF8(size_t index) const
{
    size_t byteOffset = ByteOffsetUTF8(index);
    return NextUTF8Char(byteOffset);
}

void FString::ReplaceUTF8(size_t index, unsigned unicodeChar)
{
    size_t utfPos = 0;
    size_t byteOffset = 0;
    
    while (utfPos < index && byteOffset < Length())
    {
        NextUTF8Char(byteOffset);
        ++utfPos;
    }
    
    if (utfPos < index)
        return;
    
    size_t beginCharPos = byteOffset;
    NextUTF8Char(byteOffset);
    
    char temp[7];
    char* dest = temp;
    EncodeUTF8(dest, unicodeChar);
    *dest = 0;
    
    Replace(beginCharPos, byteOffset - beginCharPos, temp, dest - temp);
}

FString& FString::AppendUTF8(unsigned unicodeChar)
{
    char temp[7];
    char* dest = temp;
    EncodeUTF8(dest, unicodeChar);
    *dest = 0;
    return Append(temp);
}

FString FString::SubstringUTF8(size_t pos) const
{
    size_t utf8Length = LengthUTF8();
    size_t byteOffset = ByteOffsetUTF8(pos);
    FString ret;

    while (pos < utf8Length)
    {
        ret.AppendUTF8(NextUTF8Char(byteOffset));
        ++pos;
    }

    return ret;
}

FString FString::SubstringUTF8(size_t pos, size_t numChars) const
{
    size_t utf8Length = LengthUTF8();
    size_t byteOffset = ByteOffsetUTF8(pos);
    size_t endPos = pos + numChars;
    FString ret;

    while (pos < endPos && pos < utf8Length)
    {
        ret.AppendUTF8(NextUTF8Char(byteOffset));
        ++pos;
    }

    return ret;
}

size_t FString::CStringLength(const char* str)
{
    if (!str)
        return 0;
    #ifdef _MSC_VER
    return strlen(str);
    #else
    const char* _ptr = str;
    while (*_ptr)
        ++_ptr;
    return _ptr - str;
    #endif
}

unsigned FString::CaseSensitiveHash(const char* str)
{
    unsigned hash = 0;
    while (*str)
    {
        hash = *str + (hash << 6) + (hash << 16) - hash;
        ++str;
    }
    
    return hash;
}

unsigned FString::CaseInsensitiveHash(const char* str)
{
    unsigned hash = 0;
    while (*str)
    {
        hash = (Auto3D::ToLower(*str)) + (hash << 6) + (hash << 16) - hash;
        ++str;
    }
    
    return hash;
}

bool FString::ToBool(const char* str)
{
    size_t length = CStringLength(str);
    
    for (size_t i = 0; i < length; ++i)
    {
        char c = Auto3D::ToLower(str[i]);
        if (c == 't' || c == 'y' || c == '1')
            return true;
        else if (c != ' ' && c != '\t')
            break;
    }
    
    return false;
}

int FString::ToInt(const char* str)
{
    if (!str)
        return 0;
    
    // Explicitly ask for base 10 to prevent source starts with '0' or '0x' from being converted to base 8 or base 16, respectively
    return strtol(str, 0, 10);
}

unsigned FString::ToUInt(const char* str)
{
    if (!str)
        return 0;
    
    return strtoul(str, 0, 10);
}

float FString::ToFloat(const char* str)
{
    if (!str)
        return 0;
    
    return (float)strtod(str, 0);
}

size_t FString::CountElements(const char* buffer, char separator)
{
    if (!buffer)
        return 0;
    
    const char* endPos = buffer + FString::CStringLength(buffer);
    const char* pos = buffer;
    size_t ret = 0;
    
    while (pos < endPos)
    {
        if (*pos != separator)
            break;
        ++pos;
    }
    
    while (pos < endPos)
    {
        const char* start = pos;
        
        while (start < endPos)
        {
            if (*start == separator)
                break;
            
            ++start;
        }
        
        if (start == endPos)
        {
            ++ret;
            break;
        }
        
        const char* end = start;
        
        while (end < endPos)
        {
            if (*end != separator)
                break;
            
            ++end;
        }
        
        ++ret;
        pos = end;
    }
    
    return ret;
}

TVector<FString> FString::Split(const char* str, char separator)
{
    TVector<FString> ret;
    size_t pos = 0;
    size_t length = CStringLength(str);
    
    while (pos < length)
    {
        if (str[pos] != separator)
            break;
        ++pos;
    }
    
    while (pos < length)
    {
        size_t start = pos;
        
        while (start < length)
        {
            if (str[start] == separator)
                break;
            
            ++start;
        }
        
        if (start == length)
        {
            ret.Push(FString(&str[pos]));
            break;
        }
        
        size_t end = start;
        
        while (end < length)
        {
            if (str[end] != separator)
                break;
            
            ++end;
        }
        
        ret.Push(FString(&str[pos], start - pos));
        pos = end;
    }
    
    return ret;
}

int FString::Compare(const char* lhs, const char* rhs, bool caseSensitive)
{
    if (!lhs || !rhs)
        return lhs ? 1 : (rhs ? -1 : 0);
    
    if (caseSensitive)
        return strcmp(lhs, rhs);
    else
    {
        for (;;)
        {
            char l = Auto3D::ToLower(*lhs);
            char r = Auto3D::ToLower(*rhs);
            if (!l || !r)
                return l ? 1 : (r ? -1 : 0);
            if (l < r)
                return -1;
            if (l > r)
                return 1;
            
            ++lhs;
            ++rhs;
        }
    }
}

void FString::EncodeUTF8(char*& dest, unsigned unicodeChar)
{
    if (unicodeChar < 0x80)
        *dest++ = (char)unicodeChar;
    else if (unicodeChar < 0x800)
    {
        dest[0] = 0xc0 | ((unicodeChar >> 6) & 0x1f);
        dest[1] = 0x80 | (unicodeChar & 0x3f);
        dest += 2;
    }
    else if (unicodeChar < 0x10000)
    {
        dest[0] = 0xe0 | ((unicodeChar >> 12) & 0xf);
        dest[1] = 0x80 | ((unicodeChar >> 6) & 0x3f);
        dest[2] = 0x80 | (unicodeChar & 0x3f);
        dest += 3;
    }
    else if (unicodeChar < 0x200000)
    {
        dest[0] = 0xf0 | ((unicodeChar >> 18) & 0x7);
        dest[1] = 0x80 | ((unicodeChar >> 12) & 0x3f);
        dest[2] = 0x80 | ((unicodeChar >> 6) & 0x3f);
        dest[3] = 0x80 | (unicodeChar & 0x3f);
        dest += 4;
    }
    else if (unicodeChar < 0x4000000)
    {
        dest[0] = 0xf8 | ((unicodeChar >> 24) & 0x3);
        dest[1] = 0x80 | ((unicodeChar >> 18) & 0x3f);
        dest[2] = 0x80 | ((unicodeChar >> 12) & 0x3f);
        dest[3] = 0x80 | ((unicodeChar >> 6) & 0x3f);
        dest[4] = 0x80 | (unicodeChar & 0x3f);
        dest += 5;
    }
    else
    {
        dest[0] = 0xfc | ((unicodeChar >> 30) & 0x1);
        dest[1] = 0x80 | ((unicodeChar >> 24) & 0x3f);
        dest[2] = 0x80 | ((unicodeChar >> 18) & 0x3f);
        dest[3] = 0x80 | ((unicodeChar >> 12) & 0x3f);
        dest[4] = 0x80 | ((unicodeChar >> 6) & 0x3f);
        dest[5] = 0x80 | (unicodeChar & 0x3f);
        dest += 6;
    }
}

#define GET_NEXT_CONTINUATION_BYTE(ptr) *ptr; if ((unsigned char)*ptr < 0x80 || (unsigned char)*ptr >= 0xc0) return '?'; else ++ptr;

unsigned FString::DecodeUTF8(const char*& src)
{
    if (src == 0)
        return 0;
    
    unsigned char char1 = *src++;
    
    // Check if we are in the middle of a UTF8 character
    if (char1 >= 0x80 && char1 < 0xc0)
    {
        while ((unsigned char)*src >= 0x80 && (unsigned char)*src < 0xc0)
            ++src;
        return '?';
    }
    
    if (char1 < 0x80)
        return char1;
    else if (char1 < 0xe0)
    {
        unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
        return (char2 & 0x3f) | ((char1 & 0x1f) << 6);
    }
    else if (char1 < 0xf0)
    {
        unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char3 = GET_NEXT_CONTINUATION_BYTE(src);
        return (char3 & 0x3f) | ((char2 & 0x3f) << 6) | ((char1 & 0xf) << 12);
    }
    else if (char1 < 0xf8)
    {
        unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char3 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char4 = GET_NEXT_CONTINUATION_BYTE(src);
        return (char4 & 0x3f) | ((char3 & 0x3f) << 6) | ((char2 & 0x3f) << 12) | ((char1 & 0x7) << 18);
    }
    else if (char1 < 0xfc)
    {
        unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char3 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char4 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char5 = GET_NEXT_CONTINUATION_BYTE(src);
        return (char5 & 0x3f) | ((char4 & 0x3f) << 6) | ((char3 & 0x3f) << 12) | ((char2 & 0x3f) << 18) | ((char1 & 0x3) << 24);
    }
    else
    {
        unsigned char char2 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char3 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char4 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char5 = GET_NEXT_CONTINUATION_BYTE(src);
        unsigned char char6 = GET_NEXT_CONTINUATION_BYTE(src);
        return (char6 & 0x3f) | ((char5 & 0x3f) << 6) | ((char4 & 0x3f) << 12) | ((char3 & 0x3f) << 18) | ((char2 & 0x3f) << 24) |
            ((char1 & 0x1) << 30);
    }
}

#ifdef _WIN32
void FString::EncodeUTF16(wchar_t*& dest, unsigned unicodeChar)
{
    if (unicodeChar < 0x10000)
        *dest++ = (wchar_t)unicodeChar;
    else
    {
        unicodeChar -= 0x10000;
        *dest++ = 0xd800 | ((unicodeChar >> 10) & 0x3ff);
        *dest++ = 0xdc00 | (unicodeChar & 0x3ff);
    }
}

unsigned FString::DecodeUTF16(const wchar_t*& src)
{
    if (src == 0)
        return 0;
    
    unsigned short word1 = *src;
    
    // Check if we are at a low surrogate
    word1 = *src++;
    if (word1 >= 0xdc00 && word1 < 0xe000)
    {
        while (*src >= 0xdc00 && *src < 0xe000)
            ++src;
        return '?';
    }
    
    if (word1 < 0xd800 || word1 >= 0xe00)
        return word1;
    else
    {
        unsigned short word2 = *src++;
        if (word2 < 0xdc00 || word2 >= 0xe000)
        {
            --src;
            return '?';
        }
        else
            return (((word1 & 0x3ff) << 10) | (word2 & 0x3ff)) + 0x10000;
    }
}
#endif

void FString::Replace(size_t pos, size_t numChars, const char* srcStart, size_t srcLength)
{
    int delta = (int)srcLength - (int)numChars;

    if (pos + numChars < Length())
    {
        if (delta < 0)
        {
            MoveRange(pos + srcLength, pos + numChars, Length() - pos - numChars);
            Resize(Length() + delta);
        }
        if (delta > 0)
        {
            Resize(Length() + delta);
            MoveRange(pos + srcLength, pos + numChars, Length() - pos - numChars - delta);
        }
    }
    else
        Resize(Length() + delta);

    CopyChars(Buffer() + pos, srcStart, srcLength);
}

void FString::MoveRange(size_t dest, size_t src, size_t numChars)
{
    if (numChars)
        memmove(Buffer() + dest, Buffer() + src, numChars);
}

void FString::CopyChars(char* dest, const char* src, size_t numChars)
{
    #ifdef _MSC_VER
    if (numChars)
        memcpy(dest, src, numChars);
    #else
    char* end = dest + numChars;
    while (dest != end)
    {
        *dest = *src;
        ++dest;
        ++src;
    }
    #endif
}

size_t FString::ListIndex(const FString& value, const FString* strings, size_t defaultIndex, bool caseSensitive)
{
    return ListIndex(value.CString(), strings, defaultIndex, caseSensitive);
}

size_t FString::ListIndex(const char* value, const FString* strings, size_t defaultIndex, bool caseSensitive)
{
    size_t i = 0;

    while (!strings[i].IsEmpty())
    {
        if (!strings[i].Compare(value, caseSensitive))
            return i;
        ++i;
    }

    return defaultIndex;
}

size_t FString::ListIndex(const FString& value, const char** strings, size_t defaultIndex, bool caseSensitive)
{
    return ListIndex(value.CString(), strings, defaultIndex, caseSensitive);
}

size_t FString::ListIndex(const char* value, const char** strings, size_t defaultIndex, bool caseSensitive)
{
    size_t i = 0;

    while (strings[i])
    {
        if (!FString::Compare(value, strings[i], caseSensitive))
            return i;
        ++i;
    }

    return defaultIndex;
}

FString FString::Format(const char* formatString, ...)
{
	FString ret;
	va_list args;
	va_start(args, formatString);
	ret.AppendWithFormatArgs(formatString, args);
	va_end(args);
	return ret;
}

FString FString::FormatV(const char* formatString, va_list args)
{
	FString ret;
	ret.AppendWithFormatArgs(formatString, args);
	return ret;
}

template<> void Swap<FString>(FString& first, FString& second)
{
    first.Swap(second);
}

}
