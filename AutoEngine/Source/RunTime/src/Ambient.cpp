#include "Ambient.h"
#include "NewDef.h"

namespace Auto3D {
Ambient::Ambient()
{
}

Ambient::~Ambient()
{

	RemoveSubSystem("Input");
	RemoveSubSystem("Renderer");
	RemoveSubSystem("Graphics");
	RemoveSubSystem("Script");
	RemoveSubSystem("IO");
	RemoveSubSystem("UI");
	RemoveSubSystem("Audio");
	RemoveSubSystem("BaseSpace");
	RemoveSubSystem("Time");

	_subSystems.clear();
	_factories.clear();
}

SharedPtr<Object> Ambient::CreateObject(STRING objectType)
{
	HASH_MAP<STRING, SharedPtr<ObjectFactory> >::const_iterator i = _factories.find(objectType);
	if (i != _factories.end())
		return i->second->CreateObject();
	else
		return SharedPtr<Object>();
}

void Ambient::RegisterSubSystem(SharedPtr<Object> object)
{
	if (!object)
		return;
	_subSystems[object->GetClassString()] = object;
}
void Ambient::RemoveSubSystem(STRING objectType) 
{
	HASH_MAP<STRING, SharedPtr<Object> >::iterator i = _subSystems.find(objectType);
	if (i != _subSystems.end())
		_subSystems.erase(i);
}
SharedPtr<Object> Ambient::GetSubSystem(STRING type)const
{
	HASH_MAP<STRING, SharedPtr<Object> >::const_iterator it 
		= _subSystems.find(type);
	if (it != _subSystems.end())
		return it->second;
	else
		return nullptr;
}
void Ambient::RegisterFactory(SharedPtr<ObjectFactory> factory)
{
	if (!factory)
		return;
	_factories[factory->GetClassString()] = factory;
}

void Ambient::RegisterFactory(SharedPtr<ObjectFactory> factory, const char* category)
{
	if (!factory)
		return;
	RegisterFactory(factory);

	_objectAttachs[category].push_back(factory->GetClassString());
}



}