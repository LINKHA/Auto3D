#include "String.h"
#include "WString.h"

#include <cstring>

#include "Debug/DebugNew.h"

namespace Auto3D
{

FWString::FWString() :
    _length(0),
    _buffer(nullptr)
{
}

FWString::FWString(const FString& str) :
    _length(0),
    _buffer(nullptr)
{
    #ifdef _WIN32
    size_t neededSize = 0;
    wchar_t temp[3];
    
    size_t byteOffset = 0;
    while (byteOffset < str.Length())
    {
        wchar_t* dest = temp;
        FString::EncodeUTF16(dest, str.NextUTF8Char(byteOffset));
        neededSize += dest - temp;
    }
    
    Resize(neededSize);
    
    byteOffset = 0;
    wchar_t* dest = _buffer;
    while (byteOffset < str.Length())
        FString::EncodeUTF16(dest, str.NextUTF8Char(byteOffset));
    #else
    Resize(str.LengthUTF8());
    
    size_t byteOffset = 0;
    wchar_t* dest = _buffer;
    while (byteOffset < str.Length())
        *dest++ = str.NextUTF8Char(byteOffset);
    #endif
}

FWString::~FWString()
{
    delete[] _buffer;
}

void FWString::Resize(size_t newLength)
{
    if (!newLength)
    {
        delete[] _buffer;
        _buffer = nullptr;
        _length = 0;
    }
    else
    {
        wchar_t* newBuffer = new wchar_t[newLength + 1];
        if (_buffer)
        {
            size_t copyLength = _length < newLength ? _length : newLength;
            memcpy(newBuffer, _buffer, copyLength * sizeof(wchar_t));
            delete[] _buffer;
        }
        newBuffer[newLength] = 0;
        _buffer = newBuffer;
        _length = newLength;
    }
}

}
