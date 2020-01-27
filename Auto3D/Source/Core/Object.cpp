#include "Object.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

THashMap<FString, SPtr<FObjectFactory> > OObject::_factories;

OObject* OObject::NewObject(FString type)
{
	auto it = _factories.Find(type);
	return it != _factories.End() ? it->_second->Create() : nullptr;
}

void OObject::RegisterFactory(SPtr<FObjectFactory> factory)
{
	if (!factory)
		return;

	_factories[factory->GetTypeName()] = factory;
}

}
