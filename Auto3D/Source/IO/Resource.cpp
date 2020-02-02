#include "Debug/Log.h"
#include "Resource.h"
#include "Debug/DebugNew.h"

namespace Auto3D
{

bool OResource::BeginLoad(const FString& pathName)
{
    return false;
}

bool OResource::EndLoad()
{
    // Resources that do not need access to main-thread critical objects do not need to override this
    return true;
}

bool OResource::Save(FStream&)
{
    ErrorString("Save not supported for " + GetTypeName());
    return false;
}

bool OResource::Load(const FString& pathName)
{
    bool success = BeginLoad(pathName);
    if (success)
        success &= EndLoad();

    return success;
}

void OResource::SetName(const FString& newName)
{
    _pathName = newName;
}
void OResource::SetMemoryUse(unsigned _size)
{
	_memoryUse = _size;
}

}
