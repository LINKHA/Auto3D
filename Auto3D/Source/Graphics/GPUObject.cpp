#include "GPUObject.h"
#include "Graphics.h"
#include "Core/Modules/ModuleManager.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

GPUObject::GPUObject() :
    _dataLost(false)
{
	_graphics = ModuleManager::Get().GraphicsModule();
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

void GPUObject::ClearDataLost()
{
	_dataLost = false;
}

Graphics* GPUObject::GetGraphics() const
{
	return _graphics;
}

}

