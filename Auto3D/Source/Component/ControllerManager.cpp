#include "Component/ControllerManager.h"
#include "Gameplay/Actor.h"
#include "Debug/Log.h"
namespace Auto3D
{
IMPLEMENT_SINGLETON(GControllerManager)

bool GControllerManager::RegisterController(AController* controller)
{
	if (!controller)
	{
		ErrorString("Fail register controller");
		return false;
	}

	unsigned id = controller->GetOwner()->GetActorID();
	_controllers[id] = controller;
	return true;
}

}