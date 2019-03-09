#include "../Debug/Log.h"
#include "Resource.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

bool Resource::BeginLoad(Stream&)
{
    return false;
}

bool Resource::EndLoad()
{
    // Resources that do not need access to main-thread critical objects do not need to override this
    return true;
}

bool Resource::Save(Stream&)
{
    ErrorString("Save not supported for " + TypeName());
    return false;
}

bool Resource::Load(Stream& source)
{
    bool success = BeginLoad(source);
    if (success)
        success &= EndLoad();

    return success;
}

void Resource::SetName(const String& newName)
{
    _name = newName;
    _nameHash = StringHash(newName);
}
void Resource::SetMemoryUse(unsigned _size)
{
	_memoryUse = _size;
}

}
