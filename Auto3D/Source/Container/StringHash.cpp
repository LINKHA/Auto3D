#include "StringHash.h"
#include "Math/MathDef.h"

#include <cstdio>

namespace Auto3D
{

const StringHash StringHash::ZERO;

String StringHash::ToString() const
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%08X", _value);
    return String(tempBuffer);
}

unsigned StringHash::Calculate(const char* str, unsigned hash)
{
	if (!str)
		return hash;

	while (*str)
	{
		hash = SDBMHash(hash, (unsigned char)*str++);
	}

	return hash;
}

}