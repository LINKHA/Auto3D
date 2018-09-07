#include "ScriptComponent.h"
#include "Ambient.h"

namespace Auto3D {
ScriptComponent::ScriptComponent(Ambient* ambient)
	:Super(ambient)
{
}


ScriptComponent::~ScriptComponent()
{
}
#if PtrDebug
SharedPtr<Object> ScriptComponent::CreateObject(_String type)
{
	return _ambient->CreateObject(type);
}
#else
Object* ScriptComponent::CreateObject(_String type)
{
	return _ambient->CreateObject(type);
}
#endif



}