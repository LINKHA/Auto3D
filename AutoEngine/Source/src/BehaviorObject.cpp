#include "BehaviorObject.h"
#include "Ambient.h"
#include "NewDef.h"

namespace Auto3D {

BehaviorObject::BehaviorObject(SharedPtr<Ambient> ambient)
	: Super(ambient)
	, _isEnable(true)
{
}


BehaviorObject::~BehaviorObject()
{
}

SharedPtr<Object> BehaviorObject::CreateObject(STRING type)
{
	return _ambient->CreateObject(type);
}



}