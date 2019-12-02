#pragma once
#include "AutoConfig.h"

namespace Auto3D
{

class AUTO_API AClass
{

};

/// Helper template allocate and construct a AClass
AUTO_API void GetPrivateStaticClassBody(
	const char* Name,
	AClass*& ReturnClass,
	void(*RegisterNativeFunc)(),
	size_t InSize);

}