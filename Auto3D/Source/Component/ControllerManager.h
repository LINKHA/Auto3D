#pragma once
#include "AutoConfig.h"
#include "Container/Singleton.h"
#include "Component/ControllerComponent.h"
#include "Container/HashMap.h"

namespace Auto3D
{

class AUTO_API GControllerManager
{
	REGISTER_SINGLETON(GControllerManager)
public:

	THashMap<unsigned, AControllerComponent*>& GetControllers() { return _controllers; }

	bool RegisterController(AControllerComponent* controller);
private:
	THashMap<unsigned, AControllerComponent*> _controllers;
};

}