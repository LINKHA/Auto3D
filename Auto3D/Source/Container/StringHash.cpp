#include "StringHash.h"

#include <cstdio>

#include "Debug/DebugNew.h"

namespace Auto3D
{

const StringHash StringHash::ZERO;

String StringHash::ToString() const
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%08X", _value);
    return String(tempBuffer);
}

}
