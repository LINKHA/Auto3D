#include "RigidBody2D.h"
#include "PhysicsWorld2D.h"
#include "SpatialNode2D.h"

#include "../Engine/ModuleManager.h"
#include "../Physics/PhysicsUtils.h"
#include "../Physics/Physics.h"


namespace Auto3D {

RigidBody2D::RigidBody2D():
	_body(nullptr)
{
	_physicsWorld2d = ModuleManager::Get().PhysicsModule()->GetActivePhysicsWorld2D();


}
RigidBody2D::~RigidBody2D()
{

}

void RigidBody2D::RegisterObject()
{
	RegisterFactory<RigidBody2D>();
}

void RigidBody2D::AddBodyToWorld()
{
	if (!_physicsWorld2d)
		return;

	if (_body)
	{
		RemoveBodyFromWorld();
	}
	else
	{
		SpatialNode2D* parentNode = dynamic_cast<SpatialNode2D*>(Parent());
		Vector3F nodePosition = parentNode->GetPosition();
		_bodyDef.position.Set(nodePosition._x, nodePosition._y);

		_body = _physicsWorld2d->GetWorld()->CreateBody(&_bodyDef);
	}
}

void RigidBody2D::RemoveBodyFromWorld()
{

}

}