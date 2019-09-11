#include "GPUObject.h"
#include "Graphics.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

GPUObject::GPUObject() :
    _dataLost(false)
{
    _graphics = Object::Module<Graphics>();
    if (_graphics)
        _graphics->AddGPUObject(this);
}

GPUObject::~GPUObject()
{
    if (_graphics)
        _graphics->RemoveGPUObject(this);
}

void GPUObject::Release()
{
}

void GPUObject::Recreate()
{
}

}

