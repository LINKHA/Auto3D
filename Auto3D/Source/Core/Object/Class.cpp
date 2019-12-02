#include "Class.h"

namespace Auto3D
{

void GetPrivateStaticClassBody(
	const char* Name,
	AClass*& ReturnClass,
	void(*RegisterNativeFunc)(),
	size_t InSize)
{

	// Register the class's native functions.
	RegisterNativeFunc();
}

}