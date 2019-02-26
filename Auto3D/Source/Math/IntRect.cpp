#include "../Base/String.h"
#include "IntRect.h"

#include <cstdio>
#include <cstdlib>

#include "../Debug/DebugNew.h"

namespace Auto3D
{

const IntRect IntRect::ZERO(0, 0, 0, 0);

bool IntRect::FromString(const String& str)
{
    return FromString(str.CString());
}

bool IntRect::FromString(const char* str)
{
    size_t elements = String::CountElements(str, ' ');
    if (elements < 4)
        return false;
    
    char* ptr = (char*)str;
    _left = strtol(ptr, &ptr, 10);
    _top = strtol(ptr, &ptr, 10);
    _right = strtol(ptr, &ptr, 10);
    _bottom = strtol(ptr, &ptr, 10);
    
    return true;
}

String IntRect::ToString() const
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%d %d %d %d", _left, _top, _right, _bottom);
    return String(tempBuffer);
}

}
