#include "Physics.h"
#include "PhysicsWorld.h"
#include "Collider.h"
#include "ColliderBox.h"

namespace Auto3D
{

Physics::Physics()
{
	RegisterSubsystem(this);
}

Physics::~Physics()
{
	RemoveSubsystem(this);
}

AUTO_API void RegisterPhysicsLibrary()
{
	static bool registered = false;
	if (registered)
		return;
	registered = true;

	PhysicsWorld::RegisterObject();
	Collider::RegisterObject();
	ColliderBox::RegisterObject();
}

}