#include "Debug/Log.h"
#include "Resource.h"
#include "Engine/Components/Font.h"
#include "Engine/Components/Sound.h"
#include "Debug/DebugNew.h"

namespace Auto3D
{
	
REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(AResource)
	.constructor<>()
	;

	// Font cpp fail,Initializes the global constructor so apply here
	REGISTER_CALSS_FACTORY_IMP(AFont)
	.constructor<>()
	.property_readonly("sdfFont", &AFont::IsSDFFont)
	;

	REGISTER_CALSS_FACTORY_IMP(ASound)
	.constructor<>()
	;
}

bool AResource::BeginLoad(FStream&)
{
    return false;
}

bool AResource::EndLoad()
{
    // Resources that do not need access to main-thread critical objects do not need to override this
    return true;
}

bool AResource::Save(FStream&)
{
    ErrorString("Save not supported for " + GetTypeName());
    return false;
}

bool AResource::Load(FStream& source)
{
    bool success = BeginLoad(source);
    if (success)
        success &= EndLoad();

    return success;
}

void AResource::SetName(const FString& newName)
{
    _name = newName;
    _nameHash = FStringHash(newName);
}
void AResource::SetMemoryUse(unsigned _size)
{
	_memoryUse = _size;
}

}
