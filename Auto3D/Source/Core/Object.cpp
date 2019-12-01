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
		if (current->GetType() == type)
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
THashMap<FStringHash, TAutoPtr<IObjectFactory> > AObject::_factories;

IObjectFactory::~IObjectFactory()
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

void AObject::SubscribeToEvent(FEvent& event, IEventHandler* handler)
{
    event.Subscribe(handler);
}

void AObject::UnsubscribeFromEvent(FEvent& event)
{
    event.Unsubscribe(this);
}

void AObject::SendEvent(FEvent& event)
{
    event.Send(this);
}

bool AObject::IsSubscribedToEvent(const FEvent& event) const
{
    return event.HasReceiver(this);
}

void AObject::RegisterObjectModule(AObject* subsystem)
{
    if (!subsystem)
        return;
    
    _objectModules[subsystem->GetType()] = subsystem;
}

void AObject::RemoveObjectModule(AObject* subsystem)
{
    if (!subsystem)
        return;
    
    auto it = _objectModules.Find(subsystem->GetType());
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

void AObject::RegisterFactory(IObjectFactory* factory)
{
    if (!factory)
        return;
    
    _factories[factory->GetType()] = factory;
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
