#include "Ambient.h"
#include "DebugNew.h"

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

#if SharedPtrDebug
sharedPtr<Object> Ambient::CreateObject(STRING objectType)
{
	HASH_MAP(STRING, sharedPtr<ObjectFactory>)::const_iterator i = _factories.find(objectType);
	if (i != _factories.end())
		return i->second->CreateObject();
	else
		return sharedPtr<Object>();
}
#else
Object* Ambient::CreateObject(STRING objectType)
{
	HASH_MAP<STRING, SharedPtr<ObjectFactory>>::const_iterator i = _factories.find(objectType);
	if (i != _factories.end())
		return i->second->CreateObject();
	else
		return nullptr;
}
#endif

void Ambient::RegisterSubSystem(Object* object)
{
	if (!object)
		return;
	_subSystems[object->GetClassString()] = SharedPtr<Object>(object);
}
void Ambient::RemoveSubSystem(STRING objectType) 
{
	HASH_MAP<STRING, SharedPtr<Object>>::iterator i = _subSystems.find(objectType);
	if (i != _subSystems.end())
		_subSystems.erase(i);
}
Object* Ambient::GetSubSystem(STRING type)const 
{
	HASH_MAP<STRING, SharedPtr<Object>>::const_iterator it 
		= _subSystems.find(type);
	if (it != _subSystems.end())
		return it->second.get();
	else
		return nullptr;
}
void Ambient::RegisterFactory(ObjectFactory* factory)
{
	if (!factory)
		return;
	_factories[factory->GetClassString()] = SharedPtr<ObjectFactory>(factory);
}

void Ambient::RegisterFactory(ObjectFactory * factory, const char* category)
{
	if (!factory)
		return;
	RegisterFactory(factory);
	//if (String::CharPtrLength(category))
		_objectAttachs[category].push_back(factory->GetClassString());
}



}