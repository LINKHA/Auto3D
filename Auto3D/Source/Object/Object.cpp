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


HashMap<StringHash, Object*> Object::_modules;
HashMap<StringHash, AutoPtr<ObjectFactory> > Object::_factories;

ObjectFactory::~ObjectFactory()
{
}

bool Object::IsInstanceOf(StringHash type) const
{
	return GetTypeInfo()->IsTypeOf(type);
}

bool Object::IsInstanceOf(const TypeInfo* typeInfo) const
{
	return GetTypeInfo()->IsTypeOf(typeInfo);
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

void Object::RegisterModule(Object* subsystem)
{
    if (!subsystem)
        return;
    
    _modules[subsystem->GetType()] = subsystem;
}

void Object::RemoveModule(Object* subsystem)
{
    if (!subsystem)
        return;
    
    auto it = _modules.Find(subsystem->GetType());
    if (it != _modules.End() && it->_second == subsystem)
        _modules.Erase(it);
}

void Object::RemoveModule(StringHash type)
{
    _modules.Erase(type);
}

Object* Object::Module(StringHash type)
{
    auto it = _modules.Find(type);
    return it != _modules.End() ? it->_second : nullptr;
}

void Object::RegisterFactory(ObjectFactory* factory)
{
    if (!factory)
        return;
    
    _factories[factory->GetType()] = factory;
}

Object* Object::Create(StringHash type)
{
    auto it = _factories.Find(type);
    return it != _factories.End() ? it->_second->Create() : nullptr;
}

const String& Object::TypeNameFromType(StringHash type)
{
    auto it = _factories.Find(type);
    return it != _factories.End() ? it->_second->GetTypeName() : String::EMPTY;
}

}
