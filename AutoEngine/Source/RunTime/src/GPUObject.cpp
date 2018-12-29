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
	if (auto graphics = _graphics.lock())
	{
		graphics->AddGPUObject(SharedPtr<GPUObject>(this));
	}
	else
		ErrorString("GPU Object fail regisiter graphics");
}


GPUObject::~GPUObject()
{
	/*if (_graphics)
		_graphics->RemoveGPUObject(SharedPtr<GPUObject>(this));*/
}


}