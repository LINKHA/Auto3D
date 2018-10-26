#include "Ambient.h"
//#include "Str.h"

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

	_subSystems.Clear();
	_factories.Clear();
}

#if SharedPtrDebug
SharedPtr<Object> Ambient::CreateObject(__String objectType)
{
	AUTO_HASH_MAP(__String, SharedPtr<ObjectFactory>)::const_Iterator i = _factories.Find(objectType);
	if (i != _factories.End())
		return i->second->CreateObject();
	else
		return SharedPtr<Object>();
}
#else
Object* Ambient::CreateObject(__String objectType)
{
	AUTO_HASH_MAP(__String, SharedPtr<ObjectFactory>)::ConstIterator i = _factories.Find(objectType);
	if (i != _factories.End())
		return i->second->CreateObject();
	else
		return SharedPtr<Object>();
}
#endif

void Ambient::RegisterSubSystem(Object* object)
{
	if (!object)
		return;
	_subSystems[object->GetClassString()] = object;
}
void Ambient::RemoveSubSystem(__String objectType) 
{
	AUTO_HASH_MAP(__String, SharedPtr<Object>)::Iterator i = _subSystems.Find(objectType);
	if (i != _subSystems.End())
		_subSystems.Erase(i);
}
Object* Ambient::GetSubSystem(__String type)const 
{
	AUTO_HASH_MAP(__String, SharedPtr<Object>)::ConstIterator it 
		= _subSystems.Find(type);
	if (it != _subSystems.End())
		return it->second;
	else
		return nullptr;
}
void Ambient::RegisterFactory(ObjectFactory* factory)
{
	if (!factory)
		return;
	_factories[factory->GetClassString()] = factory;
}

void Ambient::RegisterFactory(ObjectFactory * factory, const char* category)
{
	if (!factory)
		return;
	RegisterFactory(factory);
	//if (String::CharPtrLength(category))
		_objectAttachs[category].PushBack(factory->GetClassString());
}



}