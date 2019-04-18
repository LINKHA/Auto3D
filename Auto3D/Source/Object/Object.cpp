#include "../Thread/Thread.h"
#include "Object.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

TypeInfo::TypeInfo(const char* typeName, const TypeInfo* baseTypeInfo) :
	_type(typeName),
	_typeName(typeName),
	_baseTypeInfo(baseTypeInfo)
{
}

TypeInfo::~TypeInfo() = default;

bool TypeInfo::IsTypeOf(StringHash type) const
{
	const TypeInfo* current = this;
	while (current)
	{
		if (current->GetType() == type)
			return true;

		current = current->GetBaseTypeInfo();
	}

	return false;
}

bool TypeInfo::IsTypeOf(const TypeInfo* typeInfo) const
{
	const TypeInfo* current = this;
	while (current)
	{
		if (current == typeInfo)
			return true;

		current = current->GetBaseTypeInfo();
	}

	return false;
}


HashMap<StringHash, Object*> Object::_subsystems;
HashMap<StringHash, AutoPtr<ObjectFactory> > Object::_factories;

ObjectFactory::~ObjectFactory()
{
}

void Object::SubscribeToEvent(Event& event, EventHandler* handler)
{
    event.Subscribe(handler);
}

void Object::UnsubscribeFromEvent(Event& event)
{
    event.Unsubscribe(this);
}

void Object::SendEvent(Event& event)
{
    event.Send(this);
}

bool Object::IsSubscribedToEvent(const Event& event) const
{
    return event.HasReceiver(this);
}

void Object::RegisterSubsystem(Object* subsystem)
{
    if (!subsystem)
        return;
    
    _subsystems[subsystem->GetType()] = subsystem;
}

void Object::RemoveSubsystem(Object* subsystem)
{
    if (!subsystem)
        return;
    
    auto it = _subsystems.Find(subsystem->GetType());
    if (it != _subsystems.End() && it->_second == subsystem)
        _subsystems.Erase(it);
}

void Object::RemoveSubsystem(StringHash type)
{
    _subsystems.Erase(type);
}

Object* Object::Subsystem(StringHash type)
{
    auto it = _subsystems.Find(type);
    return it != _subsystems.End() ? it->_second : nullptr;
}

void Object::RegisterFactory(ObjectFactory* factory)
{
    if (!factory)
        return;
    
    _factories[factory->Type()] = factory;
}

Object* Object::Create(StringHash type)
{
    auto it = _factories.Find(type);
    return it != _factories.End() ? it->_second->Create() : nullptr;
}

const String& Object::TypeNameFromType(StringHash type)
{
    auto it = _factories.Find(type);
    return it != _factories.End() ? it->_second->TypeName() : String::EMPTY;
}

}
