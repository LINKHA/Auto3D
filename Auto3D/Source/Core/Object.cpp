#include "Object.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{
/// Registered object factories.
static THashMap<FString, SPtr<FObjectFactory> > _factories;

void OObject::RegisterFactory(SPtr<FObjectFactory> factory)
{
	if (!factory)
		return;

	_factories[factory->GetTypeName()] = factory;
}

OObject* NewObject(FString type)
{
	auto it = _factories.Find(type);
	return it != _factories.End() ? it->_second->Create() : nullptr;
}

}
