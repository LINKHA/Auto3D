#include "Physics.h"
#include "PhysicsWorld.h"
#include "RigidBody.h"
#include "Collider.h"
#include "ColliderBox.h"
#include "ColliderSphere.h"

namespace Auto3D
{

Physics::Physics():
	_activeWorlds(nullptr)
{
	RegisterSubsystem(this);
}

Physics::~Physics()
{
	RemoveSubsystem(this);
}

void Physics::Update()
{
	if (_activeWorlds)
		_activeWorlds->Update();
}

void Physics::AddPhysicsWorld(PhysicsWorld* activeWorlds)
{
	if(activeWorlds)
		_physicsWorlds.Push(activeWorlds);
}

void Physics::RemovePhysicsWorld(PhysicsWorld* activeWorlds)
{
	if (activeWorlds)
		_physicsWorlds.Remove(activeWorlds);
}

void Physics::SetActivePhysicsWrold(PhysicsWorld* activeWorlds)
{
	if (_activeWorlds != activeWorlds)
		_activeWorlds = activeWorlds;
}

AUTO_API void RegisterPhysicsLibrary()
{
	static bool registered = false;
	if (registered)
		return;
	registered = true;

	PhysicsWorld::RegisterObject();
	RigidBody::RegisterObject();
	Collider::RegisterObject();
	ColliderBox::RegisterObject();
	ColliderSphere::RegisterObject();
}

}