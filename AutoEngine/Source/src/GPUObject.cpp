#include "GPUObject.h"
#include "Graphics.h"
#include "LogDef.h"

namespace Auto3D {

GPUObject::GPUObject(SharedPtr<Graphics> graphics)
	: _graphics(graphics)
{
#ifdef AUTO_OPENGL
	_object.name = 0;
#else
	_object.ptr = nullptr;
#endif
}


GPUObject::~GPUObject()
{
}
void GPUObject::OnDeviceLost()
{
#ifdef AUTO_OPENGL
	// On OpenGL the object has already been lost at this point; reset object name
	_object.name = 0;
#endif
}

}