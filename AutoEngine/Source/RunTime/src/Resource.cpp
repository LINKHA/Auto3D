#include "Resource.h"
#include "NewDef.h"


namespace Auto3D {

Resource::Resource(Ambient* ambient)
	:Super(ambient)
{
}


Resource::~Resource()
{
}

bool Resource::Load(Deserializer& source)
{
	bool success = BeginLoad(source);
	if (success)
		success &= EndLoad();
	SetAsyncLoadState(AsyncLoadState::Done);

	return success;
}
bool Resource::BeginLoad(Deserializer& source)
{
	// This always needs to be overridden by subclasses
	return false;
}
bool Resource::EndLoad()
{
	// If no GPU upload step is necessary, no override is necessary
	return true;
}
bool Resource::Save(Serializer& dest) const
{
	// This always needs to be overridden by subclasses
	return false;
}

void Resource::SetMemoryUse(unsigned size)
{
	_memoryUse = size;
}

void Resource::SetAsyncLoadState(AsyncLoadState newState)
{
	_asyncLoadState = newState;
}

void Resource::SetName(const STRING& name)
{
	_name = name;
}

}