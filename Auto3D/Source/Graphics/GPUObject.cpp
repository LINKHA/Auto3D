#include "GPUObject.h"
#include "Graphics.h"
#include "Core/Modules/ModuleManager.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

FGPUObject::FGPUObject() :
    _dataLost(false)
{
	_graphics = GModuleManager::Get().GraphicsModule();
    if (_graphics)
        _graphics->AddGPUObject(this);
}

FGPUObject::~FGPUObject()
{
    if (_graphics)
        _graphics->RemoveGPUObject(this);
}

void FGPUObject::Release()
{
}

void FGPUObject::Recreate()
{
}

void FGPUObject::ClearDataLost()
{
	_dataLost = false;
}

AGraphics* FGPUObject::GetGraphics() const
{
	return _graphics;
}

}

