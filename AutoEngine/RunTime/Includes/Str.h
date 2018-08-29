#pragma once

#include "Auto.h"

namespace Auto3D {

class String
{
public:
	String();
	~String();
	static unsigned CharPtrLength(const char* str) { return str ? (unsigned)strlen(str) : 0; }
};

}
