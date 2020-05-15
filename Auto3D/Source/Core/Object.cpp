#include "Object.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{
/// Registered object factories.
static THashMap<FString, SPtr<FObjectFactory> > factories;

void OObject::RegisterFactory(SPtr<FObjectFactory> factory)
{
	if (!factory)
		return;

	factories[factory->GetTypeName()] = factory;
}

OObject* NewObject(FString type)
{
	auto it = factories.Find(type);
	return it != factories.End() ? it->_second->Create() : nullptr;
}

}
