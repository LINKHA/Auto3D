#include "Physics.h"
#include "PhysicsWorld.h"
#include "RigidBody.h"
#include "Collider.h"
#include "ColliderBox.h"
#include "ColliderSphere.h"
#include "ColliderCapsule.h"
#include "ColliderCone.h"

#include "Auto2D/PhysicsWorld2D.h"
#include "Auto2D/RigidBody2D.h"
#include "Auto2D/Collider2D.h"
#include "Auto2D/ColliderBox2D.h"
#include "Auto2D/ColliderCircle2D.h"

namespace Auto3D
{

FPhysicsModule::FPhysicsModule():
	_activeWorld(nullptr),
	_activeWorld2d(nullptr)
{
}

FPhysicsModule::~FPhysicsModule()
{
}

void FPhysicsModule::Update()
{
	if (_activeWorld)
		_activeWorld->Update();

	if (_activeWorld2d)
		_activeWorld2d->Update();
}

void FPhysicsModule::AddPhysicsWorld(APhysicsWorld* physicsWorld)
{
	if (!physicsWorld)
		return;

	_physicsWorlds.Push(physicsWorld);
	SetActivePhysicsWorld(physicsWorld);
}

void FPhysicsModule::RemovePhysicsWorld(APhysicsWorld* physicsWorld)
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

void FPhysicsModule::SetActivePhysicsWorld(APhysicsWorld* physicsWorld)
{
	if (_activeWorld != physicsWorld)
		_activeWorld = physicsWorld;
}

void FPhysicsModule::AddPhysicsWorld2D(APhysicsWorld2D* physicsWorld2d)
{
	if (!physicsWorld2d)
		return;

	_physicsWorld2ds.Push(physicsWorld2d);
	SetActivePhysicsWrold2D(physicsWorld2d);
}

void FPhysicsModule::RemovePhysicsWorld2D(APhysicsWorld2D* physicsWorld2d)
{
	if (!physicsWorld2d)
		return;

	if (_activeWorld2d && _activeWorld2d == physicsWorld2d)
	{
		if(_physicsWorld2ds.Size() <= 1)
			_activeWorld2d = nullptr;
		else
			_activeWorld2d = _physicsWorld2ds[_physicsWorld2ds.Size() - 2];
	}
		

	_physicsWorld2ds.Remove(physicsWorld2d);
}

void FPhysicsModule::SetActivePhysicsWrold2D(APhysicsWorld2D* physicsWorld2d)
{
	if (_activeWorld2d != physicsWorld2d)
		_activeWorld2d = physicsWorld2d;
}

}