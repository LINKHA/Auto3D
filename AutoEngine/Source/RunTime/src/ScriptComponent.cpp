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
#if SharedPtrDebug
sharedPtr<Object> ScriptComponent::CreateObject(STRING type)
{
	return _ambient->CreateObject(type);
}
#else
Object* ScriptComponent::CreateObject(STRING type)
{
	return _ambient->CreateObject(type);
}
#endif



}