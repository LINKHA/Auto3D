#pragma once
#include "AutoConfig.h"
#include "Container/Singleton.h"
#include "Component/Controller.h"
#include "Container/HashMap.h"

namespace Auto3D
{

class AUTO_API GControllerManager
{
	REGISTER_SINGLETON(GControllerManager)
public:

	THashMap<unsigned, AController*>& GetControllers() { return _controllers; }

	bool RegisterController(AController* controller);
private:
	THashMap<unsigned, AController*> _controllers;
};

}