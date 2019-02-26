#include "../Base/String.h"
#include "IntVector2.h"

#include <cstdio>
#include <cstdlib>

#include "../Debug/DebugNew.h"

namespace Auto3D
{

const IntVector2 IntVector2::ZERO(0, 0);

bool IntVector2::FromString(const String& str)
{
    return FromString(str.CString());
}

bool IntVector2::FromString(const char* str)
{
    size_t elements = String::CountElements(str, ' ');
    if (elements < 2)
        return false;
    
    char* ptr = (char*)str;
    _x = strtol(ptr, &ptr, 10);
    _y = strtol(ptr, &ptr, 10);
    
    return true;
}

String IntVector2::ToString() const
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%d %d", _x, _y);
    return String(tempBuffer);
}

}
