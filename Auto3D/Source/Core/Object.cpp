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

bool FTypeInfo::IsTypeOf(StringHash type) const
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


HashMap<StringHash, AObject*> AObject::_objectModules;
HashMap<StringHash, AutoPtr<ObjectFactory> > AObject::_factories;

ObjectFactory::~ObjectFactory()
{
}

bool AObject::IsInstanceOf(StringHash type) const
{
	return GetTypeInfo()->IsTypeOf(type);
}

bool AObject::IsInstanceOf(const FTypeInfo* typeInfo) const
{
	return GetTypeInfo()->IsTypeOf(typeInfo);
}

void AObject::SubscribeToEvent(Event& event, EventHandler* handler)
{
    event.Subscribe(handler);
}

void AObject::UnsubscribeFromEvent(Event& event)
{
    event.Unsubscribe(this);
}

void AObject::SendEvent(Event& event)
{
    event.Send(this);
}

bool AObject::IsSubscribedToEvent(const Event& event) const
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

void AObject::RemoveObjectModule(StringHash type)
{
	_objectModules.Erase(type);
}

AObject* AObject::ObjectModule(StringHash type)
{
    auto it = _objectModules.Find(type);
    return it != _objectModules.End() ? it->_second : nullptr;
}

void AObject::RegisterFactory(ObjectFactory* factory)
{
    if (!factory)
        return;
    
    _factories[factory->GetType()] = factory;
}

AObject* AObject::Create(StringHash type)
{
    auto it = _factories.Find(type);
    return it != _factories.End() ? it->_second->Create() : nullptr;
}

const String& AObject::TypeNameFromType(StringHash type)
{
    auto it = _factories.Find(type);
    return it != _factories.End() ? it->_second->GetTypeName() : String::EMPTY;
}

}
