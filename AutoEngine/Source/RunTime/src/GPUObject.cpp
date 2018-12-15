#include "GPUObject.h"
#include "Graphics.h"
#include "LogDef.h"

namespace Auto3D {

GPUObject::GPUObject(Graphics* graphics)
	: _graphics(SharedPtr<Graphics>(graphics))
{
#ifdef AUTO_OPENGL
	_object.name = 0;
#else
	_object.ptr = nullptr;
#endif
	if (_graphics)
		_graphics->AddGPUObject(this);
	else
		ErrorString("GPU Object fail regisiter graphics");
}


GPUObject::~GPUObject()
{
	if (_graphics)
		_graphics->RemoveGPUObject(this);
}


}