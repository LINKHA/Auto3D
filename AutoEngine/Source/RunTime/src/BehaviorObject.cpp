#include "BehaviorObject.h"
#include "Ambient.h"

namespace Auto3D {

BehaviorObject::BehaviorObject(Ambient* ambient)
	:Super(ambient)
{
}


BehaviorObject::~BehaviorObject()
{
}


#if SharedPtrDebug
sharedPtr<Object> StateVessel::CreateObject(STRING type)
{
	return _ambient->CreateObject(type);
}
#else
Object* BehaviorObject::CreateObject(STRING type)
{
	return _ambient->CreateObject(type);
}
#endif



}