#include "Debug/Log.h"
#include "Resource.h"
#include "Debug/DebugNew.h"

namespace Auto3D
{

bool OResource::BeginLoad(FStream&)
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

bool OResource::Load(FStream& source)
{
    bool success = BeginLoad(source);
    if (success)
        success &= EndLoad();

    return success;
}

void OResource::SetName(const FString& newName)
{
    _name = newName;
    _nameHash = FStringHash(newName);
}
void OResource::SetMemoryUse(unsigned _size)
{
	_memoryUse = _size;
}

}
