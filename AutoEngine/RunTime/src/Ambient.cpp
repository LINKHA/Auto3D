#include "Ambient.h"


AUTO_BEGIN
Ambient::Ambient()
{
}


Ambient::~Ambient()
{
}
void Ambient::RegisterFactory(ObjectFactory* factory)
{
	
}
void Ambient::RegisterSubSystem(Object* object)
{
	if (!object)
		return;
	_subSystems[object->GetClassStringVirtual()] = object;
}
void Ambient::RemoveSubSystem(_String objectType) 
{

}
Object* Ambient::GetSubSystem(_String type)const 
{
	AUTO_HASH_MAP(_String, SharedPtr<Object>)::const_iterator it 
		= _subSystems.find(type);
	if (it != _subSystems.end())
		return it->second;
	else
		return nullptr;
}
AUTO_END