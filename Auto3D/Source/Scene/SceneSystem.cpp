#include "SceneSystem.h"


namespace Auto3D
{

SceneSystem::SceneSystem()
{
	RegisterSubsystem(this);
}
SceneSystem::~SceneSystem()
{
	RemoveSubsystem(this);
}


}