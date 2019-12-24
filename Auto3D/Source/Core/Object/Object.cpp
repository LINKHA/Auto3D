#include "Thread/Thread.h"
#include "Object.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

FTypeInfo::FTypeInfo(const char* typeName, const FTypeInfo* baseTypeInfo) :
	_type(typeName),
	_typeName(typeName),
	_baseTypeInfo(baseTypeInfo)
{
}

FTypeInfo::~FTypeInfo() = default;

bool FTypeInfo::IsTypeOf(FStringHash type) const
{
	const FTypeInfo* current = this;
	while (current)
	{
		if (current->GetTypeHash() == type)
			return true;

		current = current->GetBaseTypeInfo();
	}

	return false;
}

bool FTypeInfo::IsTypeOf(const FTypeInfo* typeInfo) const
{
	const FTypeInfo* current = this;
	while (current)
	{
		if (current == typeInfo)
			return true;

		current = current->GetBaseTypeInfo();
	}

	return false;
}


THashMap<FStringHash, AObject*> AObject::_objectModules;
THashMap<FStringHash, TAutoPtr<FObjectFactory> > AObject::_factories;

FObjectFactory::~FObjectFactory()
{
}

bool AObject::IsInstanceOf(FStringHash type) const
{
	return GetTypeInfo()->IsTypeOf(type);
}

bool AObject::IsInstanceOf(const FTypeInfo* typeInfo) const
{
	return GetTypeInfo()->IsTypeOf(typeInfo);
}

void AObject::RegisterObjectModule(AObject* subsystem)
{
    if (!subsystem)
        return;
    
    _objectModules[subsystem->GetTypeHash()] = subsystem;
}

void AObject::RemoveObjectModule(AObject* subsystem)
{
    if (!subsystem)
        return;
    
    auto it = _objectModules.Find(subsystem->GetTypeHash());
    if (it != _objectModules.End() && it->_second == subsystem)
		_objectModules.Erase(it);
}

void AObject::RemoveObjectModule(FStringHash type)
{
	_objectModules.Erase(type);
}

AObject* AObject::ObjectModule(FStringHash type)
{
    auto it = _objectModules.Find(type);
    return it != _objectModules.End() ? it->_second : nullptr;
}

void AObject::RegisterFactory(FObjectFactory* factory)
{
    if (!factory)
        return;
    
    _factories[factory->GetTypeHash()] = factory;
}

AObject* AObject::Create(FStringHash type)
{
    auto it = _factories.Find(type);
    return it != _factories.End() ? it->_second->Create() : nullptr;
}

const FString& AObject::TypeNameFromType(FStringHash type)
{
    auto it = _factories.Find(type);
    return it != _factories.End() ? it->_second->GetTypeName() : FString::EMPTY;
}

}
