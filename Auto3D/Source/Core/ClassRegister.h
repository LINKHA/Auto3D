#pragma once
#include "AutoConfig.h"

namespace Auto3D
{

///Class registrations that need to be registered under the Auto3D lib library must be registered in this class 
///to ensure that each class is properly registered.Otherwise, when the exe file does not reference the symbols 
///of some classes, automatic registration in CPP will be invalid.An exe project can be automatically registered 
///in CPP, as shown in the following example
///
///... .h
///class TestClass : public AObject
///{
///		DECLARE_CLASS(TestClass, AObject)
///
///... .cpp
///REGISTER_CLASS(TestClass)
///{
///	REGISTER_CALSS_IMP(TestClass)
///		.constructor<>()
class AUTO_API FClassRegister
{
public:
	FClassRegister()
	{
		AutoClassRegister();
	}
	static void AutoClassRegister();
};

}