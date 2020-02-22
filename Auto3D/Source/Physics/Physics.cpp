#include "Physics.h"
#include "PhysicsWorld.h"
#include "RigidBody.h"
#include "Collider.h"
#include "ColliderBox.h"
#include "ColliderSphere.h"
#include "ColliderCapsule.h"
#include "ColliderCone.h"

namespace Auto3D
{

IMPLEMENT_SINGLETON(GPhysicsModule)

GPhysicsModule::GPhysicsModule():
	_activeWorld(nullptr)
{
}

GPhysicsModule::~GPhysicsModule()
{
}

void GPhysicsModule::Update()
{
	if (_activeWorld)
		_activeWorld->Update();
}

void GPhysicsModule::AddPhysicsWorld(FPhysicsWorld* physicsWorld)
{
	if (!physicsWorld)
		return;

	_physicsWorlds.Push(physicsWorld);
	SetActivePhysicsWorld(physicsWorld);
}

void GPhysicsModule::RemovePhysicsWorld(FPhysicsWorld* physicsWorld)
{
	if (!physicsWorld)
		return;

	if (_activeWorld && _activeWorld == physicsWorld)
	{
		if (_physicsWorlds.Size() <= 1)
			_activeWorld = nullptr;
		else
			_activeWorld = _physicsWorlds[_physicsWorlds.Size() - 2];
	}

	_physicsWorlds.Remove(physicsWorld);
}

void GPhysicsModule::SetActivePhysicsWorld(FPhysicsWorld* physicsWorld)
{
	if (_activeWorld != physicsWorld)
		_activeWorld = physicsWorld;
}

}